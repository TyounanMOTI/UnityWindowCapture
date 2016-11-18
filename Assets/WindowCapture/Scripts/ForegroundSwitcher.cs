using UnityEngine;
using System.Collections;

public class ForegroundSwitcher : MonoBehaviour {
	// Update is called once per frame
	void Update () {
		var texture = GetComponent<WindowCapture.Texture>();
		var foreground = WindowCapture.Manager.foreground;
		if (foreground != null) {
			texture.window = foreground;
		}
	}
}
