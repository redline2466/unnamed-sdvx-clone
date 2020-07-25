#pragma once
#include <Graphics/Mesh.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/Material.hpp>

namespace Graphics
{

	using Shared::Rect;
	/*
		Represents a draw command that can be executed in a render queue
	*/
	class RenderQueueItem
	{
	public:
		virtual ~RenderQueueItem() = default;
	};

	// Most basic draw command that only contains a material, it's parameters and a world transform
	class SimpleDrawCall : public RenderQueueItem
	{
	public:
		SimpleDrawCall();
		// The mesh to draw
		Mesh mesh;
		// Material to use
		Material mat;
		// The world transform
		Transform worldTransform; 
		// Scissor rectangle
		Rect scissorRect;
	};

	// Command for points/lines with size/width parameter
	class PointDrawCall : public RenderQueueItem
	{
	public:
		// List of points/lines
		Mesh mesh;
		Material mat;
		float size;
	};


	class TextDrawCall : public RenderQueueItem
	{
	public:
		// Text to render
		Ref<TextRes> text;
		// The world transform
		Transform worldTransform;
		// Scissor rectangle
		Rect scissorRect;
		Color color;
	};

	/*
		This class is a queue that collects draw commands
		each of these is stored together with their wanted render state.

		When Process is called, the commands are sorted and grouped, then sent to the graphics pipeline.
	*/
	class RenderQueue : public Unique
	{
	public:
		RenderQueue() = default;
		RenderQueue(OpenGL* ogl, const RenderState& rs);
		RenderQueue(RenderQueue&& other);
		RenderQueue& operator=(RenderQueue&& other);
		~RenderQueue();
		// Processes all render commands
		void Process(bool clearQueue = true);
		// Clears all the render commands in the queue
		void Clear();
		void Draw(Transform worldTransform, Mesh m, Material mat);
		void DrawText(Transform worldTransform, Ref<class TextRes> text, Color c);
		void DrawScissored(Rect scissor, Transform worldTransform, Mesh m, Material mat);
		void DrawScissoredText(Rect scissor, Transform worldTransform, Ref<class TextRes> text, Color c);

		// Draw for lines/points with point size parameter
		void DrawPoints(Mesh m, Material mat, float pointSize);

	private:
		RenderState m_renderState;
		Vector<RenderQueueItem*> m_orderedCommands;
		class OpenGL* m_ogl = nullptr;
	};
}