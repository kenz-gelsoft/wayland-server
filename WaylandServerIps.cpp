#include "WaylandServerIps.h"
#include "WaylandServer.h"

#include <wayland-server-core.h>
#include <wayland-server-protocol.h>
#include <stdio.h>
#include <new>

#include <Application.h>
#include "AppKitPtrs.h"


static void Assert(bool cond) {if (!cond) abort();}

extern "C" void
wl_client_dispatch(struct wl_client *client, struct wl_closure *closure);

typedef int (*client_enqueue_proc)(void *client_display, struct wl_closure *closure);


static struct wl_display *sDisplay;

ServerHandler gServerHandler;
BMessenger gServerMessenger;


ServerHandler::ServerHandler(): BHandler("server")
{}

void ServerHandler::MessageReceived(BMessage *msg)
{
	switch (msg->what) {
	case closureSendMsg: {
		struct wl_client *client;
		msg->FindPointer("client", (void**)&client);
		struct wl_closure *closure;
		msg->FindPointer("closure", (void**)&closure);
		wl_client_dispatch(client, closure);
		return;
	}
	default:
		break;
	}
	BHandler::MessageReceived(msg);
}


//#pragma mark - entry points

extern "C" _EXPORT int wl_ips_client_connected(void **clientOut, void *clientDisplay, client_enqueue_proc display_enqueue)
{
	if (be_app == NULL) {
		new Application();
		be_app->Run();
	}
	if (gServerHandler.Looper() == NULL) {
		AppKitPtrs::LockedPtr(be_app)->AddHandler(&gServerHandler);
		gServerMessenger.SetTo(&gServerHandler);
	}

	fprintf(stderr, "wl_ips_client_connected\n");
	if (sDisplay == NULL) {
		sDisplay = wl_display_create();

#if 0
		Assert(HaikuShmGlobal::Create(sDisplay) != NULL);
		Assert(HaikuCompositorGlobal::Create(sDisplay) != NULL);
		Assert(HaikuSubcompositorGlobal::Create(sDisplay) != NULL);
		Assert(HaikuOutputGlobal::Create(sDisplay) != NULL);
		Assert(HaikuDataDeviceManagerGlobal::Create(sDisplay) != NULL);
		Assert(HaikuSeatGlobal::Create(sDisplay) != NULL);
		Assert(HaikuXdgShell::Create(sDisplay) != NULL);
		Assert(HaikuServerDecorationManagerGlobal::Create(sDisplay) != NULL);
#endif
	}
	fprintf(stderr, "display: %p\n", sDisplay);
	struct wl_client *client = wl_client_create_ips(sDisplay, clientDisplay, display_enqueue);
	fprintf(stderr, "client: %p\n", client);
	static_cast<Application*>(be_app)->AddClient(client);

	*clientOut = client;

	return 0;
/*
	wl_client_destroy(client);
	wl_display_destroy(display);
*/
}

extern "C" _EXPORT int wl_ips_closure_send(void *clientIn, struct wl_closure *closure)
{
	struct wl_client *client = (struct wl_client*)clientIn;

	if (true) {
		BMessage msg(ServerHandler::closureSendMsg);
		msg.AddPointer("client", client);
		msg.AddPointer("closure", closure);
		gServerMessenger.SendMessage(&msg);
	} else {
		wl_client_dispatch(client, closure);
	}

	return 0;
}
