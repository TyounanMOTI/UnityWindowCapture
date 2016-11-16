using UnityEngine;
using System;
using System.Runtime.InteropServices;

namespace WindowCapture
{
	public static class Lib
	{
		[DllImport("WindowCapture")]
		public extern static int GetWindowWidth(int id);

		[DllImport("WindowCapture")]
		public extern static int GetWindowHeight(int id);

		[DllImport("WindowCapture")]
		public extern static uint GetWindowDPI(int id);

		[DllImport("WindowCapture")]
		public extern static int GetWindowIDs(IntPtr ids, int length);

		[DllImport("WindowCapture")]
		public extern static int GetNumWindows();

		[DllImport("WindowCapture")]
		public static extern int GetForegroundWindowID();

		[DllImport("WindowCapture")]
		public static extern void SetTexture(int id, IntPtr texture);

		[DllImport("WindowCapture")]
		public static extern void UpdateWindowByID(int id);

		[DllImport("WindowCapture")]
		public static extern IntPtr GetRenderEventFunc();

		public static int[] GetWindowIDArray()
		{
			var ids = new int[GetNumWindows()];
			var gch = GCHandle.Alloc(ids, GCHandleType.Pinned);
			var length = GetWindowIDs(gch.AddrOfPinnedObject(), ids.Length);
			gch.Free();

			if (length == 0 || length != ids.Length) {
				Debug.LogError("Faild to get window ID array.");
			}

			return ids;
		}
	}
}
