#include "CashoutModule.hpp"
#include "Attackable.hpp";

void CashoutModule::OnPickup(Attackable* owner, int stacks) {
}

void CashoutModule::OnRemove(Attackable* owner, int stacks) {
}

void CashoutModule::OnModifyStats(StatSheet& stats, int stacks) {
}

void CashoutModule::OnEvent(EventType type, EventContext ctx, int stacks) {
	if (type != EventType::OnKill) return;
	ctx.source->AddItem(data["id"], 1);
}
