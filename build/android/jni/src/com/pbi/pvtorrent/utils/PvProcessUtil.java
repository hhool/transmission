package com.pbi.pvtorrent.utils;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

import android.util.Log;

public class PvProcessUtil 
{
	public PvProcessUtil()
	{
	}
	
	public static void killProcess(String packageName) 
	{
		String processId = "";
		try {
			Runtime r = Runtime.getRuntime();
			Process p = r.exec("ps");
			BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
			String inline;
			while ((inline = br.readLine()) != null) 
			{
				if (packageName != null) 
				{
					if (inline.contains(packageName)) 
					{
						break;
					}
				} 
				else 
				{
					Log.e("PvTorrent_proc", "packageName is null");
				}

			}
			br.close();
			Log.i("PvTorrent_proc", "inline" + inline);
			if (inline != null) {

				StringTokenizer processInfoTokenizer = new StringTokenizer(inline);
				int count = 0;
				while (processInfoTokenizer.hasMoreTokens()) 
				{
					count++;
					processId = processInfoTokenizer.nextToken();
					if (count == 2) 
					{
						break;
					}
				}
				Log.i("PvTorrent_proc", "kill process : " + processId);
				r.exec("kill -9 " + processId);
			}
		} 
		catch (IOException ex) 
		{
			Log.e("PvTorrent_proc", "kill" + ex.getStackTrace());
		}
	}
	
	public static void execCmd(String szCmd, boolean bCatchLog)
	{
		try
		{
			Log.d("PvTorrent_proc", "execCmd=" + szCmd);
			Runtime runtime = Runtime.getRuntime();
			Process process = runtime.exec(szCmd);
			
			if(bCatchLog)
			{
				InputStream is = process.getInputStream();     
				InputStreamReader isr = new InputStreamReader(is);     
				BufferedReader br = new BufferedReader(isr);     
				String line = null;     
				while (null != (line = br.readLine())) 
				{         
					Log.e("PvTorrent_proc", line);     
				}				
			}
		}
		catch (IOException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch (Exception e) 
		{
			// TODO Auto-generated catch block
			Log.d("PvTorrent_proc", e.toString());
		}		
	}
	
	  public static String getSDRoot()
	  {
		  String pre = null;
		  String root = null;
		  try
		  {
			  Runtime r = Runtime.getRuntime();
			  Process p = r.exec("ls mnt");
			  BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));
			  String inline;
			  while ((inline = br.readLine()) != null) 
			  {
				  if (inline.contains("mmcblk")) 
				  {
					  pre = inline;
					  break;
				  }
			  }
			  br.close();
			  root = "/mnt/" + pre + "/" + pre + "p1";
		  }
		  catch(Exception e)
		  {
			  
		  }
		  return root;
	}
	   public static boolean findProcess(String processName) {
	        // 声明一个bufferedReader对象
	        BufferedReader bufferedReader = null;
	        try {
	            Process proc = Runtime.getRuntime().exec(
	                    "ps");
	            System.out.println(proc.getInputStream().toString());
	            bufferedReader = new BufferedReader(new InputStreamReader(proc
	                    .getInputStream()));
	            System.out.println(bufferedReader.readLine());
	            String line = null;
	            while ((line = bufferedReader.readLine()) != null) {
	                if (line.contains(processName)) {
	                    return true;
	                }
	            }
	            return false;
	        } catch (Exception e) {
	            e.printStackTrace();
	            return false;
	        } finally {
	            try {
	                if (bufferedReader != null) {
	                    bufferedReader.close();
	                }
	            } catch (Exception e2) {
	                e2.printStackTrace();
	            }

	        }
	    }
}
