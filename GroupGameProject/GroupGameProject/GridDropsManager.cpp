//#include "Grid.hpp"
//
//void Grid::AddDrop(Resource* drop) {
//	if (!drop) return;
//
//	vector<GridCell*> cells = FindEntityCells(drop);
//	for (GridCell* cell : cells)
//		cell->AddDrop(drop);
//}
//
//void Grid::RemoveDrop(Resource* drop) {
//	if (!drop) return;
//
//	vector<GridCell*> cells = FindEntityCells(drop);
//	for (GridCell* cell : cells)
//		cell->RemoveDrop(drop);
//
//	delete drop;
//}
//
//void Grid::SpawnDrops(ResourceDrop drop) {
//
//	//add padding to spawn range so not so compact
//	int scatter = cellSize * 0.25;
//	uniform_int_distribution<int> xPositionGen(drop.spawnerPosition.x - scatter, (drop.spawnerPosition.x + drop.spawnerSize.x) + scatter);
//	uniform_int_distribution<int> yPositionGen(drop.spawnerPosition.y - scatter, (drop.spawnerPosition.y + drop.spawnerSize.y) + scatter);
//	uniform_real_distribution<float> fallTimeGen(3, 10);
//
//	int drawSize = context.grid->GetCellSize() * .25;
//	int numDrops = drop.amount;
//	SDL_Texture* tex = nullptr;
//	Sprite* spr = new Sprite();
//	switch (drop.type) {
//	case(ResourceType::COIN):
//		//condense coins into gold if possible 
//		CondenseToGold(drop);
//
//		tex = context.txm->LoadTexture(context.renderer, "../../assets/coin.png");
//		spr->Initialize(tex, 2195, 2195, 0, 0, drawSize, drawSize);
//		break;
//	case(ResourceType::GOLD):
//		tex = context.txm->LoadTexture(context.renderer, "../../assets/gold.png");
//		spr->Initialize(tex, 302, 183, 0, 0, drawSize, drawSize);
//		break;
//	case(ResourceType::WOOD):
//		tex = context.txm->LoadTexture(context.renderer, "../../assets/log.png");
//		spr->Initialize(tex, 410, 261, 0, 0, drawSize, drawSize);
//		break;
//	case(ResourceType::STONE):
//		tex = context.txm->LoadTexture(context.renderer, "../../assets/stone.png");
//		spr->Initialize(tex, 404, 334, 0, 0, drawSize, drawSize);
//		break;
//	}
//	for (int i = 0; i < numDrops; ++i) {
//
//		//spawn slightly higher than object position to give drop affect
//		Vector2 pos = Vector2(xPositionGen(gen), yPositionGen(gen) - (cellSize * 0.25));
//
//		Resource* r = new Resource();
//		Sprite* s = spr->Clone();
//		float fallTime = fallTimeGen(gen) * (2 / (pos.y / cellSize));//higher placed drops fall longer
//		r->Initialize(pos, Vector2(0, 0), s, fallTime, drop.type);
//		CollisionShape cs = CollisionShape::MakeCircle(itemPickupRadius);
//		r->SetCollisionBound(cs);
//		UpdateDropOccupancy(r);
//	}
//}
//
//void Grid::CondenseToGold(ResourceDrop drop) {
//	ResourceDrop d = drop;
//	d.amount /= 10;
//	d.type = ResourceType::GOLD;
//	SpawnDrops(d);
//}
//
//void Grid::UpdateDropOccupancy(Resource* drop) {
//	UpdateOccupancy(drop, &GridCell::AddDrop, &GridCell::RemoveDrop);
//}
//
//void Grid::ChangeItemPickupRadius(int amount) {
//	itemPickupRadius += amount;
//}
//
//int Grid::GetItemPickupRadius() {
//	return itemPickupRadius;
//}