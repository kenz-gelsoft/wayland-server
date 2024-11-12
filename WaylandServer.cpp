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


WaylandApplication::WaylandApplication(): BApplication("application/x-vnd.Wayland-App")
{
}

void WaylandApplication::AddClient(struct wl_client *client)
{
	fClient = client;
}

thread_id WaylandApplication::Run()
{
	return BLooper::Run();
}

void WaylandApplication::Quit()
{
	BApplication::Quit();
	exit(0);
}

void WaylandApplication::MessageReceived(BMessage *msg)
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
