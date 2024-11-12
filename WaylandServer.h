#pragma once

#include <Application.h>

class Application: public BApplication {
private:
	// TODO: support multiple clients
	struct wl_client *fClient{};

public:
	Application();
	virtual ~Application() = default;

	void AddClient(struct wl_client *client);

	thread_id Run() override;
	void Quit() override;
	void MessageReceived(BMessage *msg) override;
};
