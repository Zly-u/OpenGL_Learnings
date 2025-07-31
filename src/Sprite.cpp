#include "Sprite.hpp"


Sprite::Sprite(): Renderer("shaders/vertex.glsl", "shaders/fragment.glsl") {
	Init();
}
Sprite::Sprite(const std::string_view& VertexShader, const std::string_view& FragmentShader): Renderer(VertexShader, FragmentShader) {
	Init();
}

void Sprite::Init() {

}

void Sprite::Render() {
	Renderer.Render();
}
