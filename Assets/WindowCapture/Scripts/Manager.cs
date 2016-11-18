using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace WindowCapture
{
	public class Manager : MonoBehaviour
	{
		private static Manager instance_;
		public static Manager instance
		{
			get { return CreateInstance(); }
		}

		private List<Window> windows_;
		public static List<Window> windows
		{
			get { return instance.windows_; }
		}

		public static Window foreground
		{
			get
			{
				var id = Lib.GetForegroundWindowID();
				if (id == -1) {
					return null;
				}
				var window = instance.windows_.Find(x => x.id == id);
				if (window != null) {
					return window;
				}
				var new_window = new Window(id);
				instance.windows_.Add(new_window);
				return new_window;
			}
		}

		private Coroutine renderCoroutine_ = null;

		private static Manager CreateInstance()
		{
			if (instance_) {
				return instance_;
			}

			var manager = FindObjectOfType<Manager>();
			if (manager) {
				manager.Awake();
				return manager;
			}

			var go = new GameObject("WindowCaptureManager");
			return go.AddComponent<Manager>();
		}

		void Awake()
		{
			if (instance_ != null) {
				return;
			}
			instance_ = this;

			Lib.GetForegroundWindowID();
			windows_ = Lib.GetWindowIDArray()
				.Select(x => new Window(x)).ToList();
		}

		void OnEnable()
		{
			renderCoroutine_ = StartCoroutine(OnRender());
		}

		IEnumerator OnRender()
		{
			for (;;) {
				yield return new WaitForEndOfFrame();
				foreach (var window in windows_) {
					window.Render();
				}
			}
		}

		void OnDisable()
		{
			if (renderCoroutine_ != null) {
				StopCoroutine(renderCoroutine_);
				renderCoroutine_ = null;
			}
		}
	}
}
