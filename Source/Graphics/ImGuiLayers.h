#pragma once
#include <functional>
#include <memory>
#include <vector>

#include "Utils/NonCopyable.h"


class IImGuiLayer {
public:
	virtual ~IImGuiLayer() = default;

	virtual void RenderGui() = 0;
};

class ImGuiCallbackLayer: public IImGuiLayer {
public:
	explicit ImGuiCallbackLayer(std::function<void()>&& callback);
	void RenderGui() override;


private:
	std::function<void()> _callback;
};


class ImGuiLayers: public NonMoveCopyable {
public:
	static ImGuiLayers& Get();

	static void PushLayer(std::weak_ptr<IImGuiLayer> layer);

	void Render();


private:
	std::vector<std::weak_ptr<IImGuiLayer>> _layers;
};