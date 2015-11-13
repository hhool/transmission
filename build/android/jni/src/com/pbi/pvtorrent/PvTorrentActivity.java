package com.pbi.pvtorrent;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.pbi.pvtorrent.utils.PvProcessUtil;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

public class PvTorrentActivity extends Activity {
	
    /** Called when the activity is first created. */
    @Override 
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
                
        String szSdCard  	= getExternalStoragePath();
        if(szSdCard ==null )
        {
        	Toast.makeText(this, R.string.process_startup_nosdcard, Toast.LENGTH_LONG).show();
        	finish();
        	return ;
        }
        copyHtml(this);
    	
        startPvTorrent(this);	
        
        Intent viewIntent = new    Intent("android.intent.action.VIEW",Uri.parse("http://127.0.0.1:9091"));
      
        startActivity(viewIntent);
        
        Toast.makeText(this, R.string.process_startup_sucessed, Toast.LENGTH_LONG).show();
        
        finish();
    }
    public static boolean copyAssetFileToTarget(Context context, String source, String szTargetPath, String szTargetFile)
	{
		InputStream fi = null;
		FileOutputStream fos = null;
		        
		try {
			fi = context.getAssets().open(source, Context.MODE_WORLD_WRITEABLE);
			File file = new File(szTargetPath + "/" + szTargetFile);
			
			fos = context.openFileOutput(szTargetFile, Context.MODE_WORLD_READABLE|Context.MODE_WORLD_WRITEABLE);//new FileOutputStream(target);
			int len = -1;
			byte[] bytes = new byte[10240];
			while ((len = fi.read(bytes)) != -1) 
			{
				fos.write(bytes, 0, len);
			}
			
		} catch (IOException e) {
			e.printStackTrace();
			Log.i("PvTorrentActivity", e.getMessage());
			return false;
		} finally {
			try {
				if (fi != null) {
					fi.close();
				}
				if (fos != null) {
					fos.close();
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return true;
	}
    private static void CopyAssets(Context context,String assetDir,String dir) 
    {  
	    String[] files;     
	    try      
	     {      
	         files = context.getResources().getAssets().list(assetDir);      
	     }      
	    catch (IOException e1)      
	    {      
	         return;      
	     }      
	     File mWorkingPath = new File(dir);  
	     //if this directory does not exists, make one.   
	    if(!mWorkingPath.exists())      
	    {      
	         if(!mWorkingPath.mkdirs())      
	         {      
	                  
	         }      
	     }
	    else
	    	return ;
	       
	     for(int i = 0; i < files.length; i++)      
	     {      
	         try      
	         {      
	             String fileName = files[i];   
	             //we make sure file name not contains '.' to be a folder.   
	             if(!fileName.contains("."))  
	             {  
	                 if(0==assetDir.length())  
	                 {  
	                     CopyAssets(context,fileName,dir+fileName+"/");  
	                 }  
	                 else 
	                 {  
	                     CopyAssets(context,assetDir+"/"+fileName,dir+"/"+fileName);  
	                 }  
	                 continue;  
	             }  
	             File outFile = new File(mWorkingPath, fileName);      
	             if(outFile.exists())   
	                 outFile.delete();  
	             InputStream in =null;  
	             if(0!=assetDir.length())  
	                 in = context.getAssets().open(assetDir+"/"+fileName);      
	             else 
	                 in = context.getAssets().open(fileName);  
	             OutputStream out = new FileOutputStream(outFile);      
	       
	             // Transfer bytes from in to out     
	             byte[] buf = new byte[1024];      
	             int len;      
	             while ((len = in.read(buf)) > 0)      
	             {      
	                 out.write(buf, 0, len);      
	             }      
	       
	             in.close();      
	            out.close();      
	         }      
	         catch (FileNotFoundException e)     
	        {      
	            e.printStackTrace();      
	        }      
	         catch (IOException e)      
	         {      
	             e.printStackTrace();      
	         }           
	    }
     }
  
    public static void copyHtml(Context context)
	{
    	String szTargetPath = context.getFilesDir().getPath();
    	szTargetPath +="/pvtorrent/web";
    	CopyAssets(context,"pvtorrent/web",szTargetPath);
	}
   
    public static String getExternalStoragePath() {
		boolean exists = Environment.getExternalStorageState().equals(
				android.os.Environment.MEDIA_MOUNTED);	
		
		return "/mnt/sda/sda1";
	/*
		if (exists)
			return Environment.getExternalStorageDirectory().getAbsolutePath();
		else
			return null;*/
	}
    /**
     * 5Ŀ¼
     */
    public static void createPath(String path) {
        File file = new File(path);
        if (!file.exists()) {
            file.mkdir();
        }
    }
    public static void startPvTorrent(Context context)
	{
        String szTargetPath = context.getFilesDir().getPath();
       
        // start ppc
        String szSdCard  = getExternalStoragePath();
    	if(szSdCard == null)
    		return;
		String szConfig 	= szSdCard+"/pvtorrent/config"; // -g config_dir where to find configfile
		
		String szCache  	= szSdCard;
		
		if(szCache != null)
		{
			String szDownLoad	= szCache+"/pvtorrent/download"; // -w download_dir where to save downloaded data
			String szIncoming	= szCache+"/pvtorrent/incoming"; // --incomplete-dir where to save torrent file until finish
		
			createPath(szCache+"/pvtorrent");
			createPath(szDownLoad);
			createPath(szIncoming);
						
			String szCmd=String.format("%s/transmission-daemon -f -a *.*.*.* -g %s -w %s --incomplete-dir %s", szTargetPath,  szConfig,szDownLoad,szIncoming);
			
			PvTorrent.GetInstance().Init(szCmd);
			
			Log.d("PvTorrentActivity", "startPvTorrent cmd="+szCmd);
			PvTorrent.GetInstance().StartWork();
			
			try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//PvTorrent.GetInstance().StopWork();
			
			//PvTorrent.GetInstance().UnInit();
		}
	}
    
}