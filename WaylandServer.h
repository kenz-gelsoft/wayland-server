#pragma once

#include <Application.h>

class WaylandApplication: public BApplication {
private:
	// TODO: support multiple clients
	struct wl_client *fClient{};

public:
	WaylandApplication();
	virtual ~WaylandApplication() = default;

	void AddClient(struct wl_client *client);

	thread_id Run() override;
	void Quit() override;
	void MessageReceived(BMessage *msg) override;
};
