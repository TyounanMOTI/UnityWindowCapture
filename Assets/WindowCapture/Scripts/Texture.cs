using UnityEngine;

namespace WindowCapture
{
	[AddComponentMenu("WindowCapture/Texture")]
	public class Texture : MonoBehaviour
	{
		public Material material
		{
			get;
			private set;
		}

		private Window window_;
		public Window window
		{
			get { return window_; }
			set
			{
				window_ = value;
				material = GetComponent<Renderer>().material; // clone
				material.mainTexture = window.texture;
			}
		}

		[Header("Invert UVs")]
		public bool invertX = false;
		public bool invertY = false;

		[Header("Clip")]
		public bool useClip = false;
		public Vector2 clipPos = Vector2.zero;
		public Vector2 clipScale = new Vector2(0.2f, 0.2f);

		public enum Bend
		{
			Off = 0,
			Y = 1,
			Z = 2,
		}

		public Bend bend
		{
			get
			{
				return (Bend)material.GetInt("_Bend");
			}
			set
			{
				switch (value) {
					case Bend.Off:
						material.SetInt("_Bend", 0);
						material.DisableKeyword("_BEND_OFF");
						material.DisableKeyword("_BEND_Y");
						material.DisableKeyword("_BEND_Z");
						break;
					case Bend.Y:
						material.SetInt("_Bend", 1);
						material.DisableKeyword("_BEND_OFF");
						material.EnableKeyword("_BEND_Y");
						material.DisableKeyword("_BEND_Z");
						break;
					case Bend.Z:
						material.SetInt("_Bend", 2);
						material.DisableKeyword("_BEND_OFF");
						material.DisableKeyword("_BEND_Y");
						material.EnableKeyword("_BEND_Z");
						break;
				}
			}
		}

		public float radius
		{
			get { return material.GetFloat("_Radius"); }
			set { material.SetFloat("_Radius", value); }
		}

		public float width
		{
			get { return material.GetFloat("_Width"); }
			set { material.SetFloat("_Width", value); }
		}

		public float height
		{
			get { return material.GetFloat("_Height"); }
			set { material.SetFloat("_Height", value); }
		}

		void OnEnable()
		{
			if (window == null) {
				window = Manager.foreground;
			}
		}

		void Update()
		{
			UpdateMaterial();
		}

		void UpdateMaterial()
		{
			UpdateDimension();
			Invert();
			Clip();
		}

		void UpdateDimension()
		{
			material.SetFloat("_Width", window_.widthMeter);
			material.SetFloat("_Height", window_.heightMeter);
		}

		void Invert()
		{
			if (invertX) {
				material.EnableKeyword("INVERT_X");
			} else {
				material.DisableKeyword("INVERT_X");
			}

			if (invertY) {
				material.EnableKeyword("INVERT_Y");
			} else {
				material.DisableKeyword("INVERT_Y");
			}
		}

		void Clip()
		{
			if (useClip) {
				material.EnableKeyword("USE_CLIP");
				material.SetVector("_ClipPositionScale", new Vector4(clipPos.x, clipPos.y, clipScale.x, clipScale.y));
			} else {
				material.DisableKeyword("USE_CLIP");
			}
		}
	}

}
