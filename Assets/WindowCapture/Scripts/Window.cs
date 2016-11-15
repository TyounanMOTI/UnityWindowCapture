using UnityEngine;

namespace WindowCapture
{
	public class Window
	{
		public Window(int id)
		{
			id_ = id;
		}

		private int id_;
		public int id
		{
			get { return id_; }
		}

		private int width
		{
			get { return Lib.GetWindowWidth(id); }
		}

		private int height
		{
			get { return Lib.GetWindowHeight(id); }
		}

		public float widthMeter
		{
			get { return width / Lib.GetWindowDPI(id) * 0.0254f; }
		}

		public float heightMeter
		{
			get { return height / Lib.GetWindowDPI(id) * 0.0254f; }
		}

		private Texture2D texture_;
		public Texture2D texture
		{
			get { return texture_; }
		}

		public void CreateTexture()
		{
			texture_ = Texture2D.CreateExternalTexture(
				width,
				height,
				TextureFormat.BGRA32,
				false,
				true,
				Lib.GetTexture(id)
				);
		}

		public void Render()
		{
			GL.IssuePluginEvent(Lib.GetRenderEventFunc(), id);
		}
	}
}