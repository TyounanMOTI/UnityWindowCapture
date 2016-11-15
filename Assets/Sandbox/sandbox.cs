using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class sandbox : MonoBehaviour {
	[DllImport("WindowCapture")]
	static extern int GetForegroundWindowID();

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
		Debug.Log(GetForegroundWindowID());
	}
}
