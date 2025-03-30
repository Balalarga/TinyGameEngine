#include "ImGuiLayers.h"

#include <algorithm>


ImGuiCallbackLayer::ImGuiCallbackLayer(std::function<void()>&& callback) : _callback(callback) {
}

void ImGuiCallbackLayer::RenderGui() {
	if (_callback)
		_callback();
}

ImGuiLayers& ImGuiLayers::Get() {
	static ImGuiLayers instance;
	return instance;
}

void ImGuiLayers::PushLayer(std::weak_ptr<IImGuiLayer> layer) {
	auto& self = Get();
	if (!layer.expired() && layer.lock().get())
		self._layers.push_back(std::move(layer));
}

void ImGuiLayers::Render() {
	std::ranges::remove_if(
		_layers,
		[](const std::weak_ptr<IImGuiLayer>& layer) {
			return layer.expired();
		});

	for (auto& weakLayer: _layers) {
		const std::shared_ptr<IImGuiLayer> layer = weakLayer.lock();
		if (!layer) {
			continue;
		}

		layer->RenderGui();
	}
}