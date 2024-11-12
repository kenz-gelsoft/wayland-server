#include "WaylandServer.h"
#include "WaylandServerIps.h"
#include "Wayland.h"
#include "HaikuShm.h"
#include "HaikuCompositor.h"
#include "HaikuSubcompositor.h"
#include "HaikuXdgShell.h"
#include "HaikuXdgSurface.h"
#include "HaikuXdgToplevel.h"
#include "HaikuOutput.h"
#include "HaikuDataDeviceManager.h"
#include "HaikuSeat.h"
#include "HaikuServerDecoration.h"
#include "WaylandEnv.h"

static void Assert(bool cond) {if (!cond) abort();}


Application::Application(): BApplication("application/x-vnd.Wayland-App")
{
}

void Application::AddClient(struct wl_client *client)
{
	fClient = client;
}

thread_id Application::Run()
{
	return BLooper::Run();
}

void Application::Quit()
{
	BApplication::Quit();
	exit(0);
}

void Application::MessageReceived(BMessage *msg)
{
	switch (msg->what) {
	case B_KEY_MAP_LOADED:
		if (fClient == NULL) return;
		WaylandEnv env(this);
		HaikuSeatGlobal *seat = HaikuGetSeat(fClient);
		if (seat == NULL) return;
		seat->UpdateKeymap();
		return;
	}
	return BApplication::MessageReceived(msg);
}
