package com.pbi.pvtorrent;

import android.util.Log;

public class PvTorrent {
	
	private final static String TAG = "transmission-daemon";
	static PvTorrent m_instance=null;
	public 	native int 		Init(String Cmd);
	private native int 		Start();
	private native boolean 	IsRun();
	private native void 	Stop();
	public 	native int 		UnInit();
	
	
	static
	{
		try 
	   	{
			System.loadLibrary("trdaemon-jni");
	   	}
		catch (UnsatisfiedLinkError ule)
	   	{
	   		Log.e(TAG, "Could not load library: " + ule);
		}
	   	catch (SecurityException se) 
	   	{
            Log.e(TAG, "Encountered a security issue when loading library: " + se);
        }
	}
	
	public static  PvTorrent GetInstance()
	{
		if(m_instance == null)
		{
			m_instance = new PvTorrent();	
		}
		return m_instance;
	}
	public boolean IsRunning()
	{
		if(m_instance == null)
			return false;
		
		return m_instance.IsRun();
	}
	public boolean StartWork()
	{
		if(m_instance == null)
			return false;
		
		if(!m_instance.IsRun())
		{
			m_instance.Start();
		}
			
		return true;
	}
	public void StopWork()
	{
		if(m_instance == null)
			return ;
		
		if(m_instance.IsRun())
		{
			m_instance.Stop();
		}
	}

}
