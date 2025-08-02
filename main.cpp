#include "App.hpp"
#include "Logging.h"

// TODO: Post-Processing
// TODO: Better way handling custom shaders and their uniforms.

int main() {
	App& app = App::Get();

	auto InitResult = app.Init();
	if (!InitResult.has_value())
	{
		Log::println("[ERROR] {}", InitResult.error());
		return -1;
	}

	app.PostInit();

	app.GameLoop();

	return 0;
}
