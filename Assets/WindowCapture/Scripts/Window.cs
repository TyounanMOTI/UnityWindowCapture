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
			get
			{
				if (!texture_) {
					CreateTexture();
				}
				return texture_;
			}
		}

		public void CreateTexture()
		{
			if (width == 0 || height == 0) {
				return;
			}
			texture_ = new Texture2D(
				width,
				height,
				TextureFormat.BGRA32,
				false
				);
			Lib.SetTexture(id, texture_.GetNativeTexturePtr());
		}

		public void Render()
		{
			if (width == 0 || height == 0) {
				// 最小化状態からの復帰時はサイズが０になる
				return;
			}
			if (texture.width != width
				|| texture.height != height) {
				CreateTexture();
			}
			GL.IssuePluginEvent(Lib.GetRenderEventFunc(), id);
		}
	}
}