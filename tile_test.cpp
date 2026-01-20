
// // // // // Game Name : Reflex Rift

// // // // #include <SDL2/SDL.h>
// // // // #include <SDL2/SDL_image.h>
// // // // #include <SDL2/SDL_ttf.h>
// // // // #include <nlohmann/json.hpp>

// // // // #include <vector>
// // // // #include <fstream>
// // // // #include <iostream>
// // // // #include <cmath>
// // // // #include <random>
// // // // #include <algorithm>

// // // // using json = nlohmann::json;

// // // // /* ---------------------------------------------------
// // // //    CONSTANTS & CONFIG
// // // // --------------------------------------------------- */
// // // // constexpr int WINDOW_WIDTH = 1280;
// // // // constexpr int WINDOW_HEIGHT = 720;

// // // // constexpr int TILE_SIZE = 16;
// // // // constexpr int TILE_SCALE = 2;
// // // // constexpr int TILE_WORLD = TILE_SIZE * TILE_SCALE;

// // // // constexpr int TILESET_TILE_SIZE = 16;
// // // // constexpr int TILESET_COLUMNS = 9;

// // // // // Physics
// // // // constexpr float GRAVITY = 0.981f;
// // // // constexpr float MOVE_SPEED = 6.0f;
// // // // constexpr float JUMP_FORCE = -21.0f;
// // // // constexpr float DASH_SPEED = 15.0f;
// // // // constexpr float DASH_DURATION = 0.2f;
// // // // constexpr float DASH_COOLDOWN = 1.0f;

// // // // // Combat
// // // // constexpr float MELEE_RANGE = 40.0f;
// // // // constexpr float MELEE_COOLDOWN = 0.4f;
// // // // constexpr float SHOOT_COOLDOWN = 0.6f;
// // // // constexpr float PROJECTILE_SPEED = 10.0f;

// // // // // Entity Config
// // // // constexpr float ENEMY_SPEED = 2.0f;
// // // // constexpr float FLY_SPEED = 3.0f;
// // // // constexpr float TURRET_RATE = 2.0f;

// // // // // Boss Config
// // // // constexpr int BOSS_HP_MAX = 50;
// // // // constexpr float BOSS_SPEED = 3.5f;

// // // // // Visuals
// // // // constexpr int ANIM_SPEED_PLAYER = 120;
// // // // constexpr int ANIM_SPEED_FIRE = 150;
// // // // constexpr float SHAKE_DURATION = 0.5f;
// // // // constexpr float FADE_SPEED = 1500.0f; // NEW: Much faster transitions

// // // // /* ---------------------------------------------------
// // // //    ENUMS
// // // // --------------------------------------------------- */
// // // // enum class GameState
// // // // {
// // // //     Menu,
// // // //     Playing,
// // // //     Transition,
// // // //     GameOver,
// // // //     Win
// // // // };

// // // // enum class TileType
// // // // {
// // // //     Ground,
// // // //     Spike,
// // // //     Checkpoint,
// // // //     Fire,
// // // //     Key,
// // // //     Breakable,
// // // //     LevelEnd,
// // // //     Spring,
// // // //     EnemyPatrol,
// // // //     EnemyFly,
// // // //     Turret,
// // // //     MovingPlatform,
// // // //     FallingPlatform,
// // // //     EnemyShield,
// // // //     Boss
// // // // };

// // // // enum class MapContext
// // // // {
// // // //     Main,
// // // //     Hidden
// // // // };
// // // // enum class PlayerAnim
// // // // {
// // // //     Idle,
// // // //     Run,
// // // //     Jump,
// // // //     Crouch,
// // // //     Dash,
// // // //     Attack
// // // // };
// // // // enum class BossState
// // // // {
// // // //     Idle,
// // // //     Charge,
// // // //     Shoot
// // // // };

// // // // /* ---------------------------------------------------
// // // //    STRUCTS
// // // // --------------------------------------------------- */
// // // // struct Tile
// // // // {
// // // //     float x{}, y{};
// // // //     TileType type{};
// // // //     bool active{true};
// // // //     float minX{}, maxX{}, speed{};
// // // //     int dir{1};
// // // //     float fallTimer{0.0f};
// // // //     bool shaking{false}, falling{false};
// // // //     float originalX{}, originalY{};
// // // // };

// // // // struct Particle
// // // // {
// // // //     float x, y, vx, vy, life;
// // // //     SDL_Color color;
// // // // };

// // // // struct Projectile
// // // // {
// // // //     float x, y, vx, vy;
// // // //     bool active;
// // // //     bool fromPlayer;
// // // // };

// // // // struct Enemy
// // // // {
// // // //     float x, y, vx, vy;
// // // //     float startX;
// // // //     int w = TILE_WORLD, h = TILE_WORLD;
// // // //     TileType type;
// // // //     bool active = true;
// // // //     float timer = 0.0f;
// // // //     int hp = 1;
// // // //     bool facingRight = false;
// // // // };

// // // // struct Boss
// // // // {
// // // //     float x, y, vx, vy;
// // // //     int w = 64, h = 64; // Big Boss
// // // //     int hp = BOSS_HP_MAX;
// // // //     BossState state = BossState::Idle;
// // // //     float stateTimer = 0.0f;
// // // //     bool active = false;
// // // // };

// // // // struct Player
// // // // {
// // // //     float x{}, y{};
// // // //     float vx{}, vy{};
// // // //     int w{TILE_WORLD}, h{TILE_WORLD};
// // // //     int hp = 3;

// // // //     bool onGround{false};
// // // //     int jumpsLeft{2};
// // // //     bool facingRight{true};

// // // //     bool isCrouching{false};
// // // //     bool isDashing{false};
// // // //     float dashTimer{0.0f};
// // // //     float dashCooldown{0.0f};

// // // //     // Combat
// // // //     float attackTimer{0.0f};      // For cooldown
// // // //     float meleeVisualTimer{0.0f}; // For animation
// // // //     float shootTimer{0.0f};

// // // //     PlayerAnim anim{PlayerAnim::Idle};
// // // //     int animFrame{0};
// // // //     Uint32 lastAnimTick{0};
// // // // };

// // // // struct HiddenArea
// // // // {
// // // //     std::vector<Tile> tiles;
// // // //     std::vector<Enemy> enemies;
// // // //     SDL_Rect bounds{};
// // // // };

// // // // struct Level
// // // // {
// // // //     std::string name;
// // // //     std::vector<Tile> tiles;
// // // //     std::vector<Enemy> enemies;
// // // //     std::vector<HiddenArea> hiddenAreas;
// // // //     Boss boss; // One boss per level container
// // // //     int totalKeys{};
// // // //     bool isDark{false};
// // // // };

// // // // struct SaveData
// // // // {
// // // //     int totalDeaths = 0;
// // // //     int highestLevel = 0;
// // // //     bool gameCompleted = false;
// // // // };

// // // // /* ---------------------------------------------------
// // // //    GLOBALS
// // // // --------------------------------------------------- */
// // // // GameState currentState = GameState::Menu;
// // // // std::vector<Level> levels;
// // // // int currentLevelIndex = 0;
// // // // MapContext currentContext = MapContext::Main;
// // // // HiddenArea *activeHidden = nullptr;

// // // // Player player;
// // // // std::vector<Particle> particles;
// // // // std::vector<Projectile> projectiles;
// // // // SaveData saveData;

// // // // float cameraX = 0, cameraY = 0;
// // // // float fadeAlpha = 0.0f;
// // // // bool fadingOut = false;
// // // // int nextLevelTarget = -1;
// // // // float checkpointX = 0, checkpointY = 0;
// // // // int keysCollected = 0;
// // // // float cameraShakeTimer = 0.0f;

// // // // // Settings
// // // // bool enableLighting = true;
// // // // int menuSelection = 0;

// // // // /* ---------------------------------------------------
// // // //    UTILS
// // // // --------------------------------------------------- */
// // // // SDL_Rect getRect(float x, float y, int w, int h)
// // // // {
// // // //     return {(int)std::round(x), (int)std::round(y), w, h};
// // // // }

// // // // bool checkCollision(SDL_Rect a, SDL_Rect b)
// // // // {
// // // //     return SDL_HasIntersection(&a, &b);
// // // // }

// // // // void spawnParticles(float x, float y, int count, SDL_Color color)
// // // // {
// // // //     for (int i = 0; i < count; i++)
// // // //     {
// // // //         Particle p;
// // // //         p.x = x;
// // // //         p.y = y;
// // // //         p.vx = (rand() % 10 - 5) * 0.5f;
// // // //         p.vy = (rand() % 10 - 5) * 0.5f - 1.0f;
// // // //         p.life = 1.0f;
// // // //         p.color = color;
// // // //         particles.push_back(p);
// // // //     }
// // // // }

// // // // TileType parseType(const std::string &name)
// // // // {
// // // //     if (name == "Spring")
// // // //         return TileType::Spring;
// // // //     if (name == "EnemyPatrol")
// // // //         return TileType::EnemyPatrol;
// // // //     if (name == "EnemyFly")
// // // //         return TileType::EnemyFly;
// // // //     if (name == "EnemyShield")
// // // //         return TileType::EnemyShield;
// // // //     if (name == "Turret")
// // // //         return TileType::Turret;
// // // //     if (name == "Boss")
// // // //         return TileType::Boss;
// // // //     if (name == "Spikes")
// // // //         return TileType::Spike;
// // // //     if (name == "Checkpoints")
// // // //         return TileType::Checkpoint;
// // // //     if (name == "Fire")
// // // //         return TileType::Fire;
// // // //     if (name == "Keys")
// // // //         return TileType::Key;
// // // //     if (name == "Breakable")
// // // //         return TileType::Breakable;
// // // //     if (name == "LevelEnd")
// // // //         return TileType::LevelEnd;
// // // //     if (name == "MovingPlatforms")
// // // //         return TileType::MovingPlatform;
// // // //     if (name == "FallingPlatform")
// // // //         return TileType::FallingPlatform;
// // // //     return TileType::Ground;
// // // // }

// // // // SDL_Rect getSrcRect(TileType type)
// // // // {
// // // //     int idx = 1;
// // // //     switch (type)
// // // //     {
// // // //     case TileType::Spring:
// // // //         idx = 8;
// // // //         break;
// // // //     case TileType::Spike:
// // // //         idx = 68;
// // // //         break;
// // // //     case TileType::Checkpoint:
// // // //         idx = 50;
// // // //         break;
// // // //     case TileType::Fire:
// // // //         idx = 54 + (SDL_GetTicks() / ANIM_SPEED_FIRE % 2);
// // // //         break;
// // // //     case TileType::Key:
// // // //         idx = 86;
// // // //         break;
// // // //     case TileType::Breakable:
// // // //         idx = 21;
// // // //         break;
// // // //     case TileType::LevelEnd:
// // // //         idx = 52;
// // // //         break;
// // // //     case TileType::MovingPlatform:
// // // //         idx = 13;
// // // //         break;
// // // //     case TileType::FallingPlatform:
// // // //         idx = 14;
// // // //         break;
// // // //     case TileType::EnemyPatrol:
// // // //         idx = 67;
// // // //         break;
// // // //     case TileType::EnemyFly:
// // // //         idx = 79;
// // // //         break;
// // // //     case TileType::EnemyShield:
// // // //         idx = 82;
// // // //         break;
// // // //     case TileType::Turret:
// // // //         idx = 80;
// // // //         break;
// // // //     case TileType::Boss:
// // // //         idx = 88;
// // // //         break;
// // // //     default:
// // // //         idx = 1;
// // // //         break;
// // // //     }
// // // //     return {(idx % TILESET_COLUMNS) * TILESET_TILE_SIZE, (idx / TILESET_COLUMNS) * TILESET_TILE_SIZE, TILESET_TILE_SIZE, TILESET_TILE_SIZE};
// // // // }

// // // // void saveGame()
// // // // {
// // // //     json j;
// // // //     j["totalDeaths"] = saveData.totalDeaths;
// // // //     j["highestLevel"] = saveData.highestLevel;
// // // //     j["gameCompleted"] = saveData.gameCompleted;
// // // //     std::ofstream o("savegame.json");
// // // //     o << j;
// // // // }

// // // // void loadSave()
// // // // {
// // // //     std::ifstream i("savegame.json");
// // // //     if (i.is_open())
// // // //     {
// // // //         json j;
// // // //         i >> j;
// // // //         saveData.totalDeaths = j.value("totalDeaths", 0);
// // // //         saveData.highestLevel = j.value("highestLevel", 0);
// // // //         saveData.gameCompleted = j.value("gameCompleted", false);
// // // //     }
// // // // }

// // // // std::vector<Level> parseMap(const std::string &path)
// // // // {
// // // //     std::vector<Level> lvls;
// // // //     std::ifstream f(path);
// // // //     if (!f.is_open())
// // // //         return lvls;
// // // //     json j;
// // // //     f >> j;

// // // //     for (auto &lj : j["levels"])
// // // //     {
// // // //         Level lvl;
// // // //         lvl.name = lj["name"];
// // // //         if (lj.contains("totalKeys"))
// // // //             lvl.totalKeys = lj["totalKeys"];
// // // //         if (lj.contains("isDark"))
// // // //             lvl.isDark = lj["isDark"];

// // // //         for (auto &layer : lj["layers"])
// // // //         {
// // // //             bool isHidden = (layer["name"] == "HiddenArea");
// // // //             HiddenArea ha;
// // // //             if (isHidden)
// // // //             {
// // // //                 auto b = layer["bounds"];
// // // //                 ha.bounds = {b["x"].get<int>() * TILE_WORLD, b["y"].get<int>() * TILE_WORLD, b["w"].get<int>() * TILE_WORLD, b["h"].get<int>() * TILE_WORLD};
// // // //             }

// // // //             for (auto &t : layer["tiles"])
// // // //             {
// // // //                 float x = t["x"].get<float>() * TILE_WORLD;
// // // //                 float y = t["y"].get<float>() * TILE_WORLD;
// // // //                 std::string typeStr = t.contains("type") ? t["type"] : (layer["name"] == "Entities" ? "Entity" : "Surface");
// // // //                 if (!t.contains("type") && layer["name"] != "Surface" && layer["name"] != "Entities" && !isHidden)
// // // //                     typeStr = layer["name"];

// // // //                 TileType type = parseType(typeStr);

// // // //                 if (type == TileType::Boss)
// // // //                 {
// // // //                     lvl.boss.active = true;
// // // //                     lvl.boss.x = x;
// // // //                     lvl.boss.y = y;
// // // //                     lvl.boss.hp = BOSS_HP_MAX;
// // // //                 }
// // // //                 else if (type == TileType::EnemyPatrol || type == TileType::EnemyFly || type == TileType::EnemyShield || type == TileType::Turret)
// // // //                 {
// // // //                     Enemy e{x, y, 0, 0, x, TILE_WORLD, TILE_WORLD, type};
// // // //                     if (isHidden)
// // // //                         ha.enemies.push_back(e);
// // // //                     else
// // // //                         lvl.enemies.push_back(e);
// // // //                 }
// // // //                 else
// // // //                 {
// // // //                     Tile tile{x, y, type};
// // // //                     tile.originalX = x;
// // // //                     tile.originalY = y;
// // // //                     if (type == TileType::MovingPlatform)
// // // //                     {
// // // //                         tile.minX = t["minX"];
// // // //                         tile.maxX = t["maxX"];
// // // //                         tile.speed = t["speed"];
// // // //                     }
// // // //                     if (isHidden)
// // // //                         ha.tiles.push_back(tile);
// // // //                     else
// // // //                         lvl.tiles.push_back(tile);
// // // //                 }
// // // //             }
// // // //             if (isHidden)
// // // //                 lvl.hiddenAreas.push_back(ha);
// // // //         }
// // // //         lvls.push_back(lvl);
// // // //     }
// // // //     return lvls;
// // // // }

// // // // /* ---------------------------------------------------
// // // //    LOGIC
// // // // --------------------------------------------------- */
// // // // void resetLevel(int idx)
// // // // {
// // // //     // If index goes beyond available levels, the game is beaten
// // // //     if (idx >= levels.size())
// // // //     {
// // // //         saveData.gameCompleted = true; // Mark game as done
// // // //         saveGame();
// // // //         currentState = GameState::Win;
// // // //         return;
// // // //     }

// // // //     currentLevelIndex = idx;
// // // //     currentContext = MapContext::Main;
// // // //     activeHidden = nullptr;
// // // //     keysCollected = 0;
// // // //     projectiles.clear();
// // // //     particles.clear();

// // // //     Level &lvl = levels[idx];
// // // //     for (auto &e : lvl.enemies)
// // // //     {
// // // //         e.active = true;
// // // //         e.x = e.startX;
// // // //         e.hp = 1;
// // // //     }
// // // //     lvl.boss.active = (lvl.boss.hp > 0); // Only active if exists
// // // //     if (lvl.boss.active)
// // // //     {
// // // //         lvl.boss.hp = BOSS_HP_MAX;
// // // //         lvl.boss.state = BossState::Idle;
// // // //     }

// // // //     for (auto &t : lvl.tiles)
// // // //     {
// // // //         if (t.type == TileType::Key)
// // // //             t.active = true;
// // // //         if (t.type == TileType::FallingPlatform)
// // // //         {
// // // //             t.active = true;
// // // //             t.y = t.originalY;
// // // //             t.falling = false;
// // // //             t.shaking = false;
// // // //         }
// // // //     }

// // // //     // Default spawn point (can be refined via map data)
// // // //     player.x = 2 * TILE_WORLD;
// // // //     player.y = 10 * TILE_WORLD;
// // // //     player.vx = 0;
// // // //     player.vy = 0;
// // // //     player.hp = 3;
// // // //     checkpointX = player.x;
// // // //     checkpointY = player.y;
// // // //     cameraX = player.x;
// // // //     cameraY = player.y;

// // // //     // Only update progress if we aren't replaying an old level
// // // //     if (idx > saveData.highestLevel)
// // // //     {
// // // //         saveData.highestLevel = idx;
// // // //     }
// // // //     saveGame();
// // // // }

// // // // void triggerTransition(int nextIdx)
// // // // {
// // // //     fadingOut = true;
// // // //     nextLevelTarget = nextIdx;
// // // // }

// // // // void respawn()
// // // // {
// // // //     saveData.totalDeaths++;
// // // //     player.x = checkpointX;
// // // //     player.y = checkpointY;
// // // //     player.vx = 0;
// // // //     player.vy = 0;
// // // //     player.hp = 3;
// // // //     currentContext = MapContext::Main;
// // // //     activeHidden = nullptr;
// // // //     spawnParticles(player.x, player.y, 15, {255, 50, 50, 255});
// // // //     fadeAlpha = 255.0f;
// // // //     fadingOut = false;
// // // //     cameraShakeTimer = 0.5f;
// // // // }

// // // // void damageEnemy(Enemy &e)
// // // // {
// // // //     e.hp--;
// // // //     if (e.hp <= 0)
// // // //     {
// // // //         e.active = false;
// // // //         spawnParticles(e.x, e.y, 10, {255, 100, 0, 255});
// // // //     }
// // // //     else
// // // //     {
// // // //         spawnParticles(e.x, e.y, 5, {200, 200, 200, 255});
// // // //     }
// // // // }

// // // // void damagePlayer()
// // // // {
// // // //     if (player.dashTimer > 0)
// // // //         return; // Invincible during dash
// // // //     player.hp--;
// // // //     cameraShakeTimer = 0.3f;
// // // //     spawnParticles(player.x, player.y, 10, {255, 0, 0, 255});
// // // //     if (player.hp <= 0)
// // // //         respawn();
// // // // }

// // // // void updatePhysics(float dt)
// // // // {
// // // //     const Uint8 *k = SDL_GetKeyboardState(nullptr);

// // // //     // Cooldowns
// // // //     if (player.dashCooldown > 0)
// // // //         player.dashCooldown -= dt;
// // // //     if (player.attackTimer > 0)
// // // //         player.attackTimer -= dt;
// // // //     if (player.meleeVisualTimer > 0)
// // // //         player.meleeVisualTimer -= dt;
// // // //     if (player.shootTimer > 0)
// // // //         player.shootTimer -= dt;

// // // //     // Dash
// // // //     if (player.isDashing)
// // // //     {
// // // //         player.dashTimer -= dt;
// // // //         if (player.dashTimer <= 0)
// // // //         {
// // // //             player.isDashing = false;
// // // //             player.vx = 0;
// // // //         }
// // // //         else
// // // //         {
// // // //             spawnParticles(player.x + player.w / 2, player.y + player.h / 2, 1, {100, 200, 255, 100});
// // // //             return;
// // // //         }
// // // //     }

// // // //     // Input
// // // //     float speed = MOVE_SPEED;
// // // //     player.isCrouching = k[SDL_SCANCODE_S] || k[SDL_SCANCODE_DOWN];
// // // //     player.h = player.isCrouching ? TILE_WORLD / 2 : TILE_WORLD;
// // // //     if (player.isCrouching)
// // // //         speed *= 0.5f;

// // // //     player.vx = 0;

// // // //     if (k[SDL_SCANCODE_LEFT])
// // // //     {
// // // //         player.vx = -speed;
// // // //         player.facingRight = false;
// // // //     }
// // // //     if (k[SDL_SCANCODE_RIGHT])
// // // //     {
// // // //         player.vx = speed;
// // // //         player.facingRight = true;
// // // //     }
// // // //     player.vy += GRAVITY;

// // // //     // --- COLLISION ---
// // // //     Level &lvl = levels[currentLevelIndex];
// // // //     auto &tiles = (currentContext == MapContext::Main) ? lvl.tiles : activeHidden->tiles;

// // // //     // Platform Logic
// // // //     for (auto &t : tiles)
// // // //     {
// // // //         if (!t.active)
// // // //             continue;
// // // //         if (t.type == TileType::MovingPlatform)
// // // //         {
// // // //             t.x += t.speed * t.dir;
// // // //             if (t.x >= t.maxX)
// // // //                 t.dir = -1;
// // // //             if (t.x <= t.minX)
// // // //                 t.dir = 1;
// // // //         }
// // // //         if (t.type == TileType::FallingPlatform && t.shaking)
// // // //         {
// // // //             t.fallTimer += dt;
// // // //             t.x = t.originalX + (rand() % 3 - 1);
// // // //             if (t.fallTimer > 0.5f)
// // // //             {
// // // //                 t.falling = true;
// // // //                 t.shaking = false;
// // // //             }
// // // //         }
// // // //         if (t.type == TileType::FallingPlatform && t.falling)
// // // //         {
// // // //             t.y += 5.0f;
// // // //             if (t.y > lvl.tiles.back().y + 500)
// // // //                 t.active = false;
// // // //         }
// // // //     }

// // // //     // X Collision
// // // //     player.x += player.vx;
// // // //     SDL_Rect pRect = getRect(player.x, player.y, player.w, player.h);
// // // //     for (auto &t : tiles)
// // // //     {
// // // //         if (!t.active || t.type == TileType::Key || t.type == TileType::Checkpoint || t.type == TileType::Spike || t.type == TileType::Fire || t.type == TileType::Spring || t.type == TileType::LevelEnd)
// // // //             continue;
// // // //         if (t.type == TileType::FallingPlatform && t.falling)
// // // //             continue;
// // // //         SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);
// // // //         if (checkCollision(pRect, tRect))
// // // //         {
// // // //             if (player.vx > 0)
// // // //                 player.x = tRect.x - player.w;
// // // //             if (player.vx < 0)
// // // //                 player.x = tRect.x + tRect.w;
// // // //             player.vx = 0;
// // // //         }
// // // //     }

// // // //     // Y Collision
// // // //     player.y += player.vy;
// // // //     player.onGround = false;
// // // //     pRect = getRect(player.x, player.y, player.w, player.h);
// // // //     for (auto &t : tiles)
// // // //     {
// // // //         if (!t.active)
// // // //             continue;
// // // //         SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);

// // // //         // Interactions
// // // //         if (t.type == TileType::Key)
// // // //         {
// // // //             if (checkCollision(pRect, tRect))
// // // //             {
// // // //                 t.active = false;
// // // //                 keysCollected++;
// // // //                 spawnParticles(t.x, t.y, 5, {255, 255, 0, 255});
// // // //             }
// // // //             continue;
// // // //         }
// // // //         if (t.type == TileType::Spring)
// // // //         {
// // // //             SDL_Rect sRect = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
// // // //             if (checkCollision(pRect, sRect) && player.vy > 0)
// // // //             {
// // // //                 player.vy = -30.0f;
// // // //                 player.jumpsLeft = 2;
// // // //                 spawnParticles(player.x, player.y + player.h, 8, {50, 255, 50, 255});
// // // //             }
// // // //             continue;
// // // //         }
// // // //         if (t.type == TileType::Spike || t.type == TileType::Fire)
// // // //         {
// // // //             SDL_Rect hurtBox = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
// // // //             if (checkCollision(pRect, hurtBox))
// // // //                 damagePlayer();
// // // //             continue;
// // // //         }
// // // //         if (t.type == TileType::LevelEnd)
// // // //         {
// // // //             if (checkCollision(pRect, tRect))
// // // //             {
// // // //                 if (currentContext == MapContext::Hidden)
// // // //                 {
// // // //                     currentContext = MapContext::Main;
// // // //                     activeHidden = nullptr;
// // // //                     player.x = checkpointX;
// // // //                     player.y = checkpointY;
// // // //                 }
// // // //                 else if (keysCollected >= lvl.totalKeys)
// // // //                     triggerTransition(currentLevelIndex + 1);
// // // //             }
// // // //             continue;
// // // //         }
// // // //         if (t.type == TileType::Checkpoint)
// // // //         {
// // // //             if (checkCollision(pRect, tRect))
// // // //             {
// // // //                 checkpointX = t.x;
// // // //                 checkpointY = t.y;
// // // //             }
// // // //             continue;
// // // //         }

// // // //         // Solids
// // // //         if (t.type == TileType::MovingPlatform)
// // // //         {
// // // //             if (player.x + player.w > t.x && player.x < t.x + TILE_WORLD && player.y + player.h >= t.y - 4 && player.y + player.h <= t.y + 10 && player.vy >= 0)
// // // //             {
// // // //                 player.y = t.y - player.h;
// // // //                 player.vy = 0;
// // // //                 player.onGround = true;
// // // //                 player.jumpsLeft = 2;
// // // //                 player.x += t.speed * t.dir;
// // // //                 continue;
// // // //             }
// // // //         }
// // // //         if (t.type == TileType::FallingPlatform && t.falling)
// // // //             continue;

// // // //         if (checkCollision(pRect, tRect))
// // // //         {
// // // //             if (t.type == TileType::FallingPlatform && player.vy > 0)
// // // //                 t.shaking = true;
// // // //             if (player.vy > 0)
// // // //             {
// // // //                 player.y = tRect.y - player.h;
// // // //                 player.vy = 0;
// // // //                 player.onGround = true;
// // // //                 player.jumpsLeft = 2;
// // // //             }
// // // //             else if (player.vy < 0)
// // // //             {
// // // //                 player.y = tRect.y + tRect.h;
// // // //                 player.vy = 0;
// // // //                 if (t.type == TileType::Breakable && !lvl.hiddenAreas.empty())
// // // //                 {
// // // //                     currentContext = MapContext::Hidden;
// // // //                     activeHidden = &lvl.hiddenAreas[0];
// // // //                     player.x = activeHidden->bounds.x + 64;
// // // //                     player.y = activeHidden->bounds.y + 64;
// // // //                 }
// // // //             }
// // // //         }
// // // //     }

// // // //     if (player.y > 2000)
// // // //         respawn();

// // // //     // --- COMBAT & ENEMIES ---
// // // //     auto &enemies = (currentContext == MapContext::Main) ? lvl.enemies : activeHidden->enemies;
// // // //     pRect = getRect(player.x, player.y, player.w, player.h);

// // // //     // Melee Hitbox
// // // //     if (player.meleeVisualTimer > 0)
// // // //     {
// // // //         SDL_Rect attackBox = {(int)(player.x + (player.facingRight ? player.w : -MELEE_RANGE)), (int)player.y, (int)MELEE_RANGE, player.h};
// // // //         for (auto &e : enemies)
// // // //         {
// // // //             if (!e.active)
// // // //                 continue;
// // // //             if (checkCollision(attackBox, getRect(e.x, e.y, e.w, e.h)))
// // // //             {
// // // //                 bool blocked = false;
// // // //                 if (e.type == TileType::EnemyShield)
// // // //                 {
// // // //                     // Block if attacking from front
// // // //                     bool hitFromLeft = player.x < e.x;
// // // //                     bool enemyFacingLeft = e.vx < 0;
// // // //                     if (hitFromLeft == enemyFacingLeft)
// // // //                         blocked = true;
// // // //                 }
// // // //                 if (blocked)
// // // //                 {
// // // //                     spawnParticles(e.x, e.y, 2, {100, 100, 255, 255});
// // // //                 }
// // // //                 else
// // // //                     damageEnemy(e);
// // // //             }
// // // //         }
// // // //         // Boss Hit
// // // //         if (lvl.boss.active && checkCollision(attackBox, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
// // // //         {
// // // //             lvl.boss.hp--;
// // // //             spawnParticles(lvl.boss.x + lvl.boss.w / 2, lvl.boss.y + lvl.boss.h / 2, 5, {255, 0, 0, 255});
// // // //             if (lvl.boss.hp <= 0)
// // // //             {
// // // //                 lvl.boss.active = false;
// // // //                 triggerTransition(currentLevelIndex + 1);
// // // //             }
// // // //         }
// // // //     }

// // // //     // Enemies Logic
// // // //     for (auto &e : enemies)
// // // //     {
// // // //         if (!e.active)
// // // //             continue;
// // // //         SDL_Rect eRect = getRect(e.x, e.y, e.w, e.h);

// // // //         if (e.type == TileType::EnemyPatrol || e.type == TileType::EnemyShield)
// // // //         {
// // // //             if (e.vx == 0)
// // // //                 e.vx = ENEMY_SPEED;
// // // //             e.x += e.vx;
// // // //             if (e.x > e.startX + 64)
// // // //                 e.vx = -ENEMY_SPEED;
// // // //             if (e.x < e.startX - 64)
// // // //                 e.vx = ENEMY_SPEED;
// // // //             if (checkCollision(pRect, eRect))
// // // //                 damagePlayer();
// // // //         }
// // // //         else if (e.type == TileType::EnemyFly)
// // // //         {
// // // //             float dist = std::sqrt(std::pow(player.x - e.x, 2) + std::pow(player.y - e.y, 2));
// // // //             if (dist < 200.0f)
// // // //             {
// // // //                 float angle = std::atan2(player.y - e.y, player.x - e.x);
// // // //                 e.x += std::cos(angle) * FLY_SPEED * 0.5f;
// // // //                 e.y += std::sin(angle) * FLY_SPEED * 0.5f;
// // // //             }
// // // //             if (checkCollision(pRect, eRect))
// // // //                 damagePlayer();
// // // //         }
// // // //         else if (e.type == TileType::Turret)
// // // //         {
// // // //             e.timer += dt;
// // // //             if (e.timer > TURRET_RATE)
// // // //             {
// // // //                 e.timer = 0;
// // // //                 Projectile p;
// // // //                 p.x = e.x;
// // // //                 p.y = e.y + 8;
// // // //                 p.vx = (player.x < e.x) ? -4.0f : 4.0f;
// // // //                 p.vy = 0;
// // // //                 p.active = true;
// // // //                 p.fromPlayer = false;
// // // //                 projectiles.push_back(p);
// // // //             }
// // // //         }
// // // //     }

// // // //     // Boss Logic
// // // //     if (lvl.boss.active)
// // // //     {
// // // //         Boss &b = lvl.boss;
// // // //         SDL_Rect bRect = getRect(b.x, b.y, b.w, b.h);
// // // //         if (checkCollision(pRect, bRect))
// // // //             damagePlayer();

// // // //         b.stateTimer += dt;
// // // //         switch (b.state)
// // // //         {
// // // //         case BossState::Idle:
// // // //             if (b.stateTimer > 2.0f)
// // // //             {
// // // //                 b.state = BossState::Charge;
// // // //                 b.stateTimer = 0;
// // // //             }
// // // //             break;
// // // //         case BossState::Charge:
// // // //             b.x += (player.x < b.x ? -BOSS_SPEED : BOSS_SPEED);
// // // //             if (b.stateTimer > 2.0f)
// // // //             {
// // // //                 b.state = BossState::Shoot;
// // // //                 b.stateTimer = 0;
// // // //             }
// // // //             break;
// // // //         case BossState::Shoot:
// // // //             if ((int)(b.stateTimer * 10) % 5 == 0)
// // // //             {
// // // //                 Projectile p;
// // // //                 p.x = b.x + b.w / 2;
// // // //                 p.y = b.y + b.h / 2;
// // // //                 p.vx = (rand() % 10 - 5);
// // // //                 p.vy = (rand() % 10 - 5);
// // // //                 p.active = true;
// // // //                 p.fromPlayer = false;
// // // //                 projectiles.push_back(p);
// // // //             }
// // // //             if (b.stateTimer > 2.0f)
// // // //             {
// // // //                 b.state = BossState::Idle;
// // // //                 b.stateTimer = 0;
// // // //             }
// // // //             break;
// // // //         }
// // // //     }

// // // //     // Projectiles
// // // //     for (auto &p : projectiles)
// // // //     {
// // // //         if (!p.active)
// // // //             continue;
// // // //         p.x += p.vx;
// // // //         p.y += p.vy;
// // // //         SDL_Rect r = getRect(p.x, p.y, 8, 8);
// // // //         if (p.fromPlayer)
// // // //         {
// // // //             for (auto &e : enemies)
// // // //             {
// // // //                 if (e.active && checkCollision(r, getRect(e.x, e.y, e.w, e.h)))
// // // //                 {
// // // //                     if (e.type != TileType::EnemyShield)
// // // //                         damageEnemy(e);
// // // //                     p.active = false;
// // // //                 }
// // // //             }
// // // //             if (lvl.boss.active && checkCollision(r, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
// // // //             {
// // // //                 lvl.boss.hp--;
// // // //                 p.active = false;
// // // //             }
// // // //         }
// // // //         else
// // // //         {
// // // //             if (checkCollision(r, pRect))
// // // //             {
// // // //                 damagePlayer();
// // // //                 p.active = false;
// // // //             }
// // // //         }
// // // //         if (std::abs(p.x - player.x) > 600)
// // // //             p.active = false;
// // // //     }

// // // //     for (auto it = particles.begin(); it != particles.end();)
// // // //     {
// // // //         it->x += it->vx;
// // // //         it->y += it->vy;
// // // //         it->life -= 0.05f;
// // // //         if (it->life <= 0)
// // // //             it = particles.erase(it);
// // // //         else
// // // //             ++it;
// // // //     }
// // // // }

// // // // /* ---------------------------------------------------
// // // //    MAIN
// // // // --------------------------------------------------- */
// // // // int main(int argc, char *argv[])
// // // // {
// // // //     SDL_Init(SDL_INIT_VIDEO);
// // // //     IMG_Init(IMG_INIT_PNG);
// // // //     TTF_Init();
// // // //     SDL_Window *window = SDL_CreateWindow("Reflex Rift", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
// // // //     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

// // // //     // CHANGE PATH IF NEEDED
// // // //     std::string path = "/Users/lazycodebaker/Documents/Dev/CPP/platformer_2d_temp/assets/";
// // // //     SDL_Texture *tilesTex = IMG_LoadTexture(renderer, (path + "spritesheet_tiles_resized.png").c_str());
// // // //     SDL_Texture *playerTex = IMG_LoadTexture(renderer, (path + "spritesheet_characters.png").c_str());
// // // //     TTF_Font *font = TTF_OpenFont((path + "PixelifySans-VariableFont_wght.ttf").c_str(), 24);

// // // //     // Lighting
// // // //     SDL_Texture *lightLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
// // // //     SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
// // // //     SDL_Texture *lightMask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 300);
// // // //     SDL_BlendMode eraser = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
// // // //     SDL_SetTextureBlendMode(lightMask, eraser);
// // // //     SDL_SetRenderTarget(renderer, lightMask);
// // // //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
// // // //     SDL_RenderClear(renderer);
// // // //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
// // // //     for (int w = 0; w < 300; w++)
// // // //         for (int h = 0; h < 300; h++)
// // // //             if ((150 - w) * (150 - w) + (150 - h) * (150 - h) < 22500)
// // // //                 SDL_RenderDrawPoint(renderer, w, h);

// // // //     SDL_SetRenderTarget(renderer, nullptr);

// // // //     loadSave();
// // // //     levels = parseMap(path + "map.json");
// // // //     // Removed debug level set, starts at menu now

// // // //     // Text Render Helper
// // // //     auto renderTextCentered = [&](std::string text, int y, SDL_Color color, float scale = 1.0f)
// // // //     {
// // // //         if (!font)
// // // //             return;
// // // //         SDL_Surface *s = TTF_RenderText_Blended(font, text.c_str(), color);
// // // //         if (!s)
// // // //             return;
// // // //         SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
// // // //         int w = (int)(s->w * scale);
// // // //         int h = (int)(s->h * scale);
// // // //         SDL_Rect dst = {WINDOW_WIDTH / 2 - w / 2, y, w, h};
// // // //         SDL_RenderCopy(renderer, t, nullptr, &dst);
// // // //         SDL_FreeSurface(s);
// // // //         SDL_DestroyTexture(t);
// // // //     };

// // // //     bool running = true;
// // // //     Uint32 lastTime = SDL_GetTicks();

// // // //     while (running)
// // // //     {
// // // //         Uint32 currentTime = SDL_GetTicks();
// // // //         float dt = (currentTime - lastTime) / 1000.0f;
// // // //         lastTime = currentTime;

// // // //         SDL_Event e;
// // // //         while (SDL_PollEvent(&e))
// // // //         {
// // // //             if (e.type == SDL_QUIT)
// // // //                 running = false;
// // // //             if (e.type == SDL_KEYDOWN)
// // // //             {
// // // //                 if (currentState == GameState::Menu)
// // // //                 {
// // // //                     if (saveData.gameCompleted)
// // // //                     {
// // // //                         // Level Select Navigation
// // // //                         if (e.key.keysym.scancode == SDL_SCANCODE_UP)
// // // //                         {
// // // //                             menuSelection--;
// // // //                             if (menuSelection < 0)
// // // //                                 menuSelection = levels.size() - 1;
// // // //                         }
// // // //                         if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
// // // //                         {
// // // //                             menuSelection++;
// // // //                             if (menuSelection >= levels.size())
// // // //                                 menuSelection = 0;
// // // //                         }
// // // //                         if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// // // //                         {
// // // //                             currentState = GameState::Playing;
// // // //                             resetLevel(menuSelection);
// // // //                         }
// // // //                     }
// // // //                     else
// // // //                     {
// // // //                         // Classic Menu Behavior
// // // //                         if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// // // //                         {
// // // //                             currentState = GameState::Playing;
// // // //                             // Reset to highest level or level 0
// // // //                             int lvl = (saveData.highestLevel < levels.size()) ? saveData.highestLevel : 0;
// // // //                             resetLevel(lvl);
// // // //                         }
// // // //                     }

// // // //                     if (e.key.keysym.scancode == SDL_SCANCODE_L)
// // // //                         enableLighting = !enableLighting;
// // // //                 }
// // // //                 else if (currentState == GameState::Win)
// // // //                 {
// // // //                     // Return to menu on Enter
// // // //                     if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// // // //                     {
// // // //                         currentState = GameState::Menu;
// // // //                     }
// // // //                 }
// // // //                 else if (currentState == GameState::Playing)
// // // //                 {
// // // //                     if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
// // // //                         currentState = GameState::Menu;
// // // //                     if (e.key.keysym.scancode == SDL_SCANCODE_SPACE && player.jumpsLeft > 0)
// // // //                     {
// // // //                         player.vy = JUMP_FORCE;
// // // //                         player.jumpsLeft--;
// // // //                         player.onGround = false;
// // // //                     }
// // // //                     if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT && !player.isDashing && player.dashCooldown <= 0)
// // // //                     {
// // // //                         player.isDashing = true;
// // // //                         player.dashTimer = DASH_DURATION;
// // // //                         player.dashCooldown = DASH_COOLDOWN;
// // // //                         player.vx = player.facingRight ? DASH_SPEED : -DASH_SPEED;
// // // //                         player.vy = 0;
// // // //                     }
// // // //                     if (e.key.keysym.scancode == SDL_SCANCODE_Z && player.attackTimer <= 0)
// // // //                     {
// // // //                         player.attackTimer = MELEE_COOLDOWN;
// // // //                         player.meleeVisualTimer = 0.1f;
// // // //                     }
// // // //                     if (e.key.keysym.scancode == SDL_SCANCODE_X && player.shootTimer <= 0)
// // // //                     {
// // // //                         player.shootTimer = SHOOT_COOLDOWN;
// // // //                         Projectile p;
// // // //                         p.x = player.x + player.w / 2;
// // // //                         p.y = player.y + player.h / 2;
// // // //                         p.vx = player.facingRight ? PROJECTILE_SPEED : -PROJECTILE_SPEED;
// // // //                         p.vy = 0;
// // // //                         p.active = true;
// // // //                         p.fromPlayer = true;
// // // //                         projectiles.push_back(p);
// // // //                     }
// // // //                 }
// // // //             }
// // // //         }

// // // //         if (currentState == GameState::Playing)
// // // //         {
// // // //             updatePhysics(dt);
// // // //             if (fadingOut)
// // // //             {
// // // //                 // Faster transition out
// // // //                 fadeAlpha += FADE_SPEED * dt;
// // // //                 if (fadeAlpha >= 255)
// // // //                 {
// // // //                     resetLevel(nextLevelTarget);
// // // //                     fadingOut = false;
// // // //                 }
// // // //             }
// // // //             else if (fadeAlpha > 0)
// // // //                 // Faster transition in
// // // //                 fadeAlpha -= FADE_SPEED * dt;

// // // //             if (cameraShakeTimer > 0)
// // // //             {
// // // //                 cameraShakeTimer -= dt;
// // // //                 cameraX += (rand() % 10 - 5);
// // // //                 cameraY += (rand() % 10 - 5);
// // // //             }
// // // //         }

// // // //         float targetCamX = player.x - WINDOW_WIDTH / 2;
// // // //         float targetCamY = player.y - WINDOW_HEIGHT / 2;
// // // //         cameraX += (targetCamX - cameraX) * 0.1f;
// // // //         cameraY += (targetCamY - cameraY) * 0.1f;

// // // //         SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
// // // //         SDL_RenderClear(renderer);

// // // //         if (currentState == GameState::Menu)
// // // //         {
// // // //             int y = 50;
// // // //             SDL_Color white = {255, 255, 255, 255};
// // // //             SDL_Color gray = {180, 180, 180, 255};
// // // //             SDL_Color gold = {255, 215, 0, 255};
// // // //             SDL_Color selectColor = {100, 255, 100, 255};

// // // //             renderTextCentered("Reflex Rift", y, white, 1.5f);
// // // //             y += 50;

// // // //             renderTextCentered("--- CONTROLS ---", y, gray, 0.8f);
// // // //             y += 25;
// // // //             renderTextCentered("Arrows: Move | Space: Jump | Shift: Dash", y, gray, 0.8f);
// // // //             y += 25;
// // // //             renderTextCentered("Z: Melee | X: Fireball", y, gray, 0.8f);
// // // //             y += 25;
// // // //             // Added Light toggle info here
// // // //             std::string lightStr = "L: Toggle Lights (" + std::string(enableLighting ? "ON" : "OFF") + ")";
// // // //             renderTextCentered(lightStr, y, gray, 0.8f);
// // // //             y += 40;

// // // //             if (saveData.gameCompleted)
// // // //             {
// // // //                 renderTextCentered("GAME COMPLETED! SELECT LEVEL:", y, gold);
// // // //                 y += 40;

// // // //                 // Render list of levels
// // // //                 for (int i = 0; i < levels.size(); i++)
// // // //                 {
// // // //                     std::string lvlName = "Level " + std::to_string(i + 1) + ": " + levels[i].name;
// // // //                     SDL_Color c = (i == menuSelection) ? selectColor : gray;
// // // //                     if (i == menuSelection)
// // // //                         lvlName = "> " + lvlName + " <";

// // // //                     renderTextCentered(lvlName, y, c);
// // // //                     y += 30;
// // // //                 }
// // // //             }
// // // //             else
// // // //             {
// // // //                 renderTextCentered("Press ENTER to Start/Continue", y, gold);
// // // //                 y += 60;
// // // //                 renderTextCentered("Objective: Collect keys to unlock the Exit", y, gray, 0.8f);
// // // //             }

// // // //             y = WINDOW_HEIGHT - 60;
// // // //             renderTextCentered("Total Deaths: " + std::to_string(saveData.totalDeaths), y, {200, 50, 50, 255});
// // // //         }
// // // //         else if (currentState == GameState::Playing)
// // // //         {
// // // //             // Parallax
// // // //             SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
// // // //             SDL_RenderClear(renderer);
// // // //             SDL_SetRenderDrawColor(renderer, 30, 33, 40, 255);
// // // //             for (int i = 0; i < 30; i++)
// // // //             {
// // // //                 SDL_Rect bgRect = {i * 200 - (int)(cameraX * 0.5f) % 3000, 100 - (int)(cameraY * 0.2f), 100, 400};
// // // //                 SDL_RenderFillRect(renderer, &bgRect);
// // // //             }

// // // //             Level &level = levels[currentLevelIndex];
// // // //             auto &tiles = (currentContext == MapContext::Main) ? level.tiles : activeHidden->tiles;
// // // //             auto &enemies = (currentContext == MapContext::Main) ? level.enemies : activeHidden->enemies;

// // // //             for (auto &t : tiles)
// // // //             {
// // // //                 if (!t.active)
// // // //                     continue;
// // // //                 SDL_Rect d = getRect(t.x - cameraX, t.y - cameraY, TILE_WORLD, TILE_WORLD);
// // // //                 SDL_Rect s = getSrcRect(t.type);
// // // //                 SDL_RenderCopy(renderer, tilesTex, &s, &d);
// // // //             }
// // // //             for (auto &e : enemies)
// // // //             {
// // // //                 if (!e.active)
// // // //                     continue;
// // // //                 SDL_Rect d = getRect(e.x - cameraX, e.y - cameraY, e.w, e.h);
// // // //                 SDL_Rect s = getSrcRect(e.type);
// // // //                 SDL_RenderCopyEx(renderer, tilesTex, &s, &d, 0, nullptr, (e.vx > 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
// // // //             }
// // // //             if (level.boss.active)
// // // //             {
// // // //                 SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
// // // //                 SDL_Rect bRect = getRect(level.boss.x - cameraX, level.boss.y - cameraY, level.boss.w, level.boss.h);
// // // //                 SDL_RenderFillRect(renderer, &bRect);
// // // //             }

// // // //             SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
// // // //             for (auto &p : projectiles)
// // // //             {
// // // //                 if (!p.active)
// // // //                     continue;
// // // //                 SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 8, 8);
// // // //                 SDL_RenderFillRect(renderer, &r);
// // // //             }
// // // //             for (auto &p : particles)
// // // //             {
// // // //                 SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, (Uint8)(p.life * 255));
// // // //                 SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 4, 4);
// // // //                 SDL_RenderFillRect(renderer, &r);
// // // //             }

// // // //             // Player & Weapon
// // // //             int pRow = (!player.onGround) ? 1 : 0;
// // // //             if (SDL_GetTicks() - player.lastAnimTick > ANIM_SPEED_PLAYER)
// // // //             {
// // // //                 player.animFrame = (player.animFrame + 1) % 2;
// // // //                 player.lastAnimTick = SDL_GetTicks();
// // // //             }
// // // //             SDL_Rect pSrc = {player.animFrame * 16, pRow * 16, 16, 16};
// // // //             if (player.isCrouching)
// // // //                 pSrc.h = 16;
// // // //             SDL_Rect pDst = getRect(player.x - cameraX, player.y - cameraY, player.w, player.h);
// // // //             SDL_RenderCopyEx(renderer, playerTex, &pSrc, &pDst, 0, nullptr, player.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

// // // //             if (player.meleeVisualTimer > 0)
// // // //             {
// // // //                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
// // // //                 SDL_Rect swing = {(int)(pDst.x + (player.facingRight ? pDst.w : -MELEE_RANGE)), pDst.y, (int)MELEE_RANGE, pDst.h};
// // // //                 SDL_RenderFillRect(renderer, &swing);
// // // //             }

// // // //             // Lighting
// // // //             if (enableLighting && level.isDark)
// // // //             {
// // // //                 SDL_SetRenderTarget(renderer, lightLayer);
// // // //                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
// // // //                 SDL_RenderClear(renderer);
// // // //                 SDL_Rect lDst = {(int)(player.x - cameraX + player.w / 2 - 150), (int)(player.y - cameraY + player.h / 2 - 150), 300, 300};
// // // //                 SDL_RenderCopy(renderer, lightMask, nullptr, &lDst);
// // // //                 for (auto &t : tiles)
// // // //                 {
// // // //                     if (t.active && t.type == TileType::Key)
// // // //                     {
// // // //                         SDL_Rect kL = {(int)(t.x - cameraX - 50), (int)(t.y - cameraY - 50), 100, 100};
// // // //                         SDL_RenderCopy(renderer, lightMask, nullptr, &kL);
// // // //                     }
// // // //                 }
// // // //                 SDL_SetRenderTarget(renderer, nullptr);
// // // //                 SDL_RenderCopy(renderer, lightLayer, nullptr, nullptr);
// // // //             }

// // // //             // HUD
// // // //             if (font)
// // // //             {
// // // //                 std::string hud = "HP: " + std::to_string(player.hp) + " | Keys: " + std::to_string(keysCollected);
// // // //                 SDL_Surface *s = TTF_RenderText_Blended(font, hud.c_str(), {255, 255, 255, 255});
// // // //                 SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
// // // //                 SDL_Rect d = {20, 20, s->w, s->h};
// // // //                 SDL_RenderCopy(renderer, t, nullptr, &d);
// // // //                 SDL_FreeSurface(s);
// // // //                 SDL_DestroyTexture(t);

// // // //                 if (level.boss.active)
// // // //                 {
// // // //                     SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
// // // //                     SDL_Rect bg = {WINDOW_WIDTH / 2 - 200, 50, 400, 20};
// // // //                     SDL_RenderFillRect(renderer, &bg);
// // // //                     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
// // // //                     SDL_Rect fg = {WINDOW_WIDTH / 2 - 200, 50, (int)(400 * ((float)level.boss.hp / BOSS_HP_MAX)), 20};
// // // //                     SDL_RenderFillRect(renderer, &fg);
// // // //                 }
// // // //             }
// // // //             if (fadeAlpha > 0)
// // // //             {
// // // //                 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
// // // //                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)fadeAlpha);
// // // //                 SDL_Rect fs = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
// // // //                 SDL_RenderFillRect(renderer, &fs);
// // // //             }
// // // //         }
// // // //         else if (currentState == GameState::Win)
// // // //         {
// // // //             renderTextCentered("YOU WIN!", WINDOW_HEIGHT / 2 - 30, {255, 215, 0, 255}, 2.0f);
// // // //             renderTextCentered("Press ENTER to return to Menu", WINDOW_HEIGHT / 2 + 40, {200, 200, 200, 255}, 1.0f);
// // // //         }

// // // //         SDL_RenderPresent(renderer);
// // // //     }

// // // //     SDL_DestroyTexture(lightLayer);
// // // //     SDL_DestroyTexture(lightMask);
// // // //     SDL_DestroyTexture(tilesTex);
// // // //     SDL_DestroyTexture(playerTex);
// // // //     TTF_CloseFont(font);
// // // //     SDL_DestroyRenderer(renderer);
// // // //     SDL_DestroyWindow(window);
// // // //     IMG_Quit();
// // // //     TTF_Quit();
// // // //     SDL_Quit();
// // // //     return 0;
// // // // }

// // // // Game Name : Reflex Rift
// // // // Date: January 20, 2026

// // // #include <SDL2/SDL.h>
// // // #include <SDL2/SDL_image.h>
// // // #include <SDL2/SDL_ttf.h>
// // // #include <nlohmann/json.hpp>

// // // #include <vector>
// // // #include <fstream>
// // // #include <iostream>
// // // #include <cmath>
// // // #include <random>
// // // #include <algorithm>

// // // using json = nlohmann::json;

// // // /* ---------------------------------------------------
// // //    CONSTANTS & CONFIG
// // // --------------------------------------------------- */
// // // constexpr int WINDOW_WIDTH = 1280;
// // // constexpr int WINDOW_HEIGHT = 720;

// // // constexpr int TILE_SIZE = 16;
// // // constexpr int TILE_SCALE = 2;
// // // constexpr int TILE_WORLD = TILE_SIZE * TILE_SCALE;

// // // constexpr int TILESET_TILE_SIZE = 16;
// // // constexpr int TILESET_COLUMNS = 9;

// // // // Physics
// // // constexpr float GRAVITY = 0.981f;
// // // constexpr float MOVE_SPEED = 6.0f;
// // // constexpr float JUMP_FORCE = -21.0f;
// // // constexpr float DASH_SPEED = 15.0f;
// // // constexpr float DASH_DURATION = 0.2f;
// // // constexpr float DASH_COOLDOWN = 1.0f;

// // // // Combat
// // // constexpr float MELEE_RANGE = 40.0f;
// // // constexpr float MELEE_COOLDOWN = 0.4f;
// // // constexpr float SHOOT_COOLDOWN = 0.6f;
// // // constexpr float PROJECTILE_SPEED = 10.0f;

// // // // Entity Config
// // // constexpr float ENEMY_SPEED = 2.0f;
// // // constexpr float FLY_SPEED = 3.0f;
// // // constexpr float TURRET_RATE = 2.0f;

// // // // Boss Config
// // // constexpr int BOSS_HP_MAX = 50;
// // // constexpr float BOSS_SPEED = 3.5f;

// // // // Visuals
// // // constexpr int ANIM_SPEED_PLAYER = 120;
// // // constexpr int ANIM_SPEED_FIRE = 150;
// // // constexpr float SHAKE_DURATION = 0.5f;
// // // constexpr float FADE_SPEED = 1500.0f;

// // // /* ---------------------------------------------------
// // //    ENUMS
// // // --------------------------------------------------- */
// // // enum class GameState
// // // {
// // //     Menu,
// // //     Playing,
// // //     Transition,
// // //     GameOver,
// // //     Win
// // // };

// // // enum class TileType
// // // {
// // //     Ground,
// // //     Spike,
// // //     Checkpoint,
// // //     Fire,
// // //     Key,
// // //     Breakable,
// // //     LevelEnd,
// // //     Spring,
// // //     EnemyPatrol,
// // //     EnemyFly,
// // //     Turret,
// // //     MovingPlatform,
// // //     FallingPlatform,
// // //     EnemyShield,
// // //     Boss
// // // };

// // // enum class MapContext
// // // {
// // //     Main,
// // //     Hidden
// // // };
// // // enum class PlayerAnim
// // // {
// // //     Idle,
// // //     Run,
// // //     Jump,
// // //     Crouch,
// // //     Dash,
// // //     Attack
// // // };
// // // enum class BossState
// // // {
// // //     Idle,
// // //     Charge,
// // //     Shoot
// // // };

// // // /* ---------------------------------------------------
// // //    STRUCTS
// // // --------------------------------------------------- */
// // // struct Tile
// // // {
// // //     float x{}, y{};
// // //     TileType type{};
// // //     bool active{true};
// // //     float minX{}, maxX{}, speed{};
// // //     int dir{1};
// // //     float fallTimer{0.0f};
// // //     bool shaking{false}, falling{false};
// // //     float originalX{}, originalY{};
// // // };

// // // struct Particle
// // // {
// // //     float x, y, vx, vy, life;
// // //     SDL_Color color;
// // // };

// // // struct Projectile
// // // {
// // //     float x, y, vx, vy;
// // //     bool active;
// // //     bool fromPlayer;
// // // };

// // // struct Enemy
// // // {
// // //     float x, y, vx, vy;
// // //     float startX;
// // //     int w = TILE_WORLD, h = TILE_WORLD;
// // //     TileType type;
// // //     bool active = true;
// // //     float timer = 0.0f;
// // //     int hp = 1;
// // //     bool facingRight = false;
// // // };

// // // struct Boss
// // // {
// // //     float x, y, vx, vy;
// // //     int w = 64, h = 64;
// // //     int hp = BOSS_HP_MAX;
// // //     BossState state = BossState::Idle;
// // //     float stateTimer = 0.0f;
// // //     bool active = false;
// // // };

// // // struct Player
// // // {
// // //     float x{}, y{};
// // //     float vx{}, vy{};
// // //     int w{TILE_WORLD}, h{TILE_WORLD};
// // //     int hp = 3;

// // //     bool onGround{false};
// // //     int jumpsLeft{2};
// // //     bool facingRight{true};

// // //     bool isCrouching{false};
// // //     bool isDashing{false};
// // //     float dashTimer{0.0f};
// // //     float dashCooldown{0.0f};

// // //     float attackTimer{0.0f};
// // //     float meleeVisualTimer{0.0f};
// // //     float shootTimer{0.0f};

// // //     PlayerAnim anim{PlayerAnim::Idle};
// // //     int animFrame{0};
// // //     Uint32 lastAnimTick{0};
// // // };

// // // struct HiddenArea
// // // {
// // //     std::vector<Tile> tiles;
// // //     std::vector<Enemy> enemies;
// // //     SDL_Rect bounds{};
// // // };

// // // struct Level
// // // {
// // //     std::string name;
// // //     std::vector<Tile> tiles;
// // //     std::vector<Enemy> enemies;
// // //     std::vector<HiddenArea> hiddenAreas;
// // //     Boss boss;
// // //     int totalKeys = 1; // FORCED TO 1
// // //     bool isDark{false};
// // // };

// // // struct SaveData
// // // {
// // //     int totalDeaths = 0;
// // //     int highestLevel = 0;
// // //     bool gameCompleted = false;
// // // };

// // // /* ---------------------------------------------------
// // //    GLOBALS
// // // --------------------------------------------------- */
// // // GameState currentState = GameState::Menu;
// // // std::vector<Level> levels;
// // // int currentLevelIndex = 0;
// // // MapContext currentContext = MapContext::Main;
// // // HiddenArea *activeHidden = nullptr;

// // // Player player;
// // // std::vector<Particle> particles;
// // // std::vector<Projectile> projectiles;
// // // SaveData saveData;

// // // float cameraX = 0, cameraY = 0;
// // // float fadeAlpha = 0.0f;
// // // bool fadingOut = false;
// // // int nextLevelTarget = -1;
// // // float checkpointX = 0, checkpointY = 0;
// // // int keysCollected = 0;
// // // float cameraShakeTimer = 0.0f;

// // // // Settings
// // // bool enableLighting = true;
// // // int menuSelection = 0;

// // // /* ---------------------------------------------------
// // //    UTILS
// // // --------------------------------------------------- */
// // // SDL_Rect getRect(float x, float y, int w, int h)
// // // {
// // //     return {(int)std::round(x), (int)std::round(y), w, h};
// // // }

// // // bool checkCollision(SDL_Rect a, SDL_Rect b)
// // // {
// // //     return SDL_HasIntersection(&a, &b);
// // // }

// // // void spawnParticles(float x, float y, int count, SDL_Color color)
// // // {
// // //     for (int i = 0; i < count; i++)
// // //     {
// // //         Particle p;
// // //         p.x = x;
// // //         p.y = y;
// // //         p.vx = (rand() % 10 - 5) * 0.5f;
// // //         p.vy = (rand() % 10 - 5) * 0.5f - 1.0f;
// // //         p.life = 1.0f;
// // //         p.color = color;
// // //         particles.push_back(p);
// // //     }
// // // }

// // // TileType parseType(const std::string &name)
// // // {
// // //     if (name == "Spring")
// // //         return TileType::Spring;
// // //     if (name == "EnemyPatrol")
// // //         return TileType::EnemyPatrol;
// // //     if (name == "EnemyFly")
// // //         return TileType::EnemyFly;
// // //     if (name == "EnemyShield")
// // //         return TileType::EnemyShield;
// // //     if (name == "Turret")
// // //         return TileType::Turret;
// // //     if (name == "Boss")
// // //         return TileType::Boss;
// // //     if (name == "Spikes")
// // //         return TileType::Spike;
// // //     if (name == "Checkpoints")
// // //         return TileType::Checkpoint;
// // //     if (name == "Fire")
// // //         return TileType::Fire;
// // //     if (name == "Keys")
// // //         return TileType::Key;
// // //     if (name == "Breakable")
// // //         return TileType::Breakable;
// // //     if (name == "LevelEnd")
// // //         return TileType::LevelEnd;
// // //     if (name == "MovingPlatforms")
// // //         return TileType::MovingPlatform;
// // //     if (name == "FallingPlatform")
// // //         return TileType::FallingPlatform;
// // //     return TileType::Ground;
// // // }

// // // SDL_Rect getSrcRect(TileType type)
// // // {
// // //     int idx = 1;
// // //     switch (type)
// // //     {
// // //     case TileType::Spring:
// // //         idx = 8;
// // //         break;
// // //     case TileType::Spike:
// // //         idx = 68;
// // //         break;
// // //     case TileType::Checkpoint:
// // //         idx = 50;
// // //         break;
// // //     case TileType::Fire:
// // //         idx = 54 + (SDL_GetTicks() / ANIM_SPEED_FIRE % 2);
// // //         break;
// // //     case TileType::Key:
// // //         idx = 86;
// // //         break;
// // //     case TileType::Breakable:
// // //         idx = 21;
// // //         break;
// // //     case TileType::LevelEnd:
// // //         idx = 52;
// // //         break;
// // //     case TileType::MovingPlatform:
// // //         idx = 13;
// // //         break;
// // //     case TileType::FallingPlatform:
// // //         idx = 14;
// // //         break;
// // //     case TileType::EnemyPatrol:
// // //         idx = 67;
// // //         break;
// // //     case TileType::EnemyFly:
// // //         idx = 79;
// // //         break;
// // //     case TileType::EnemyShield:
// // //         idx = 82;
// // //         break;
// // //     case TileType::Turret:
// // //         idx = 80;
// // //         break;
// // //     case TileType::Boss:
// // //         idx = 88;
// // //         break;
// // //     default:
// // //         idx = 1;
// // //         break;
// // //     }
// // //     return {(idx % TILESET_COLUMNS) * TILESET_TILE_SIZE, (idx / TILESET_COLUMNS) * TILESET_TILE_SIZE, TILESET_TILE_SIZE, TILESET_TILE_SIZE};
// // // }

// // // void saveGame()
// // // {
// // //     json j;
// // //     j["totalDeaths"] = saveData.totalDeaths;
// // //     j["highestLevel"] = saveData.highestLevel;
// // //     j["gameCompleted"] = saveData.gameCompleted;
// // //     std::ofstream o("savegame.json");
// // //     o << j;
// // // }

// // // void loadSave()
// // // {
// // //     std::ifstream i("savegame.json");
// // //     if (i.is_open())
// // //     {
// // //         json j;
// // //         i >> j;
// // //         saveData.totalDeaths = j.value("totalDeaths", 0);
// // //         saveData.highestLevel = j.value("highestLevel", 0);
// // //         saveData.gameCompleted = j.value("gameCompleted", false);
// // //     }
// // // }

// // // std::vector<Level> parseMap(const std::string &path)
// // // {
// // //     std::vector<Level> lvls;
// // //     std::ifstream f(path);
// // //     if (!f.is_open())
// // //         return lvls;
// // //     json j;
// // //     f >> j;

// // //     for (auto &lj : j["levels"])
// // //     {
// // //         Level lvl;
// // //         lvl.name = lj["name"];
// // //         lvl.totalKeys = 1; // Strict constraint
// // //         if (lj.contains("isDark"))
// // //             lvl.isDark = lj["isDark"];

// // //         for (auto &layer : lj["layers"])
// // //         {
// // //             bool isHidden = (layer["name"] == "HiddenArea");
// // //             HiddenArea ha;
// // //             if (isHidden)
// // //             {
// // //                 auto b = layer["bounds"];
// // //                 ha.bounds = {b["x"].get<int>() * TILE_WORLD, b["y"].get<int>() * TILE_WORLD, b["w"].get<int>() * TILE_WORLD, b["h"].get<int>() * TILE_WORLD};
// // //             }

// // //             for (auto &t : layer["tiles"])
// // //             {
// // //                 float x = t["x"].get<float>() * TILE_WORLD;
// // //                 float y = t["y"].get<float>() * TILE_WORLD;
// // //                 std::string typeStr = t.contains("type") ? t["type"] : (layer["name"] == "Entities" ? "Entity" : "Surface");
// // //                 if (!t.contains("type") && layer["name"] != "Surface" && layer["name"] != "Entities" && !isHidden)
// // //                     typeStr = layer["name"];

// // //                 TileType type = parseType(typeStr);

// // //                 if (type == TileType::Boss)
// // //                 {
// // //                     lvl.boss.active = true;
// // //                     lvl.boss.x = x;
// // //                     lvl.boss.y = y;
// // //                     lvl.boss.hp = BOSS_HP_MAX;
// // //                 }
// // //                 else if (type == TileType::EnemyPatrol || type == TileType::EnemyFly || type == TileType::EnemyShield || type == TileType::Turret)
// // //                 {
// // //                     Enemy e{x, y, 0, 0, x, TILE_WORLD, TILE_WORLD, type};
// // //                     if (isHidden)
// // //                         ha.enemies.push_back(e);
// // //                     else
// // //                         lvl.enemies.push_back(e);
// // //                 }
// // //                 else
// // //                 {
// // //                     Tile tile{x, y, type};
// // //                     tile.originalX = x;
// // //                     tile.originalY = y;
// // //                     if (type == TileType::MovingPlatform)
// // //                     {
// // //                         tile.minX = t["minX"];
// // //                         tile.maxX = t["maxX"];
// // //                         tile.speed = t["speed"];
// // //                     }
// // //                     if (isHidden)
// // //                         ha.tiles.push_back(tile);
// // //                     else
// // //                         lvl.tiles.push_back(tile);
// // //                 }
// // //             }
// // //             if (isHidden)
// // //                 lvl.hiddenAreas.push_back(ha);
// // //         }
// // //         lvls.push_back(lvl);
// // //     }
// // //     return lvls;
// // // }

// // // /* ---------------------------------------------------
// // //    LOGIC
// // // --------------------------------------------------- */
// // // void resetLevel(int idx)
// // // {
// // //     if (idx >= levels.size())
// // //     {
// // //         saveData.gameCompleted = true;
// // //         saveGame();
// // //         currentState = GameState::Win;
// // //         return;
// // //     }

// // //     currentLevelIndex = idx;
// // //     currentContext = MapContext::Main;
// // //     activeHidden = nullptr;
// // //     keysCollected = 0;
// // //     projectiles.clear();
// // //     particles.clear();

// // //     Level &lvl = levels[idx];
// // //     for (auto &e : lvl.enemies)
// // //     {
// // //         e.active = true;
// // //         e.x = e.startX;
// // //         e.hp = 1;
// // //     }
// // //     lvl.boss.active = (lvl.boss.hp > 0);
// // //     if (lvl.boss.active)
// // //     {
// // //         lvl.boss.hp = BOSS_HP_MAX;
// // //         lvl.boss.state = BossState::Idle;
// // //     }

// // //     for (auto &t : lvl.tiles)
// // //     {
// // //         if (t.type == TileType::Key)
// // //             t.active = true;
// // //         if (t.type == TileType::FallingPlatform)
// // //         {
// // //             t.active = true;
// // //             t.y = t.originalY;
// // //             t.falling = false;
// // //             t.shaking = false;
// // //         }
// // //     }

// // //     // Default Spawn
// // //     player.x = 2 * TILE_WORLD;
// // //     player.y = 10 * TILE_WORLD;

// // //     // Level 2 special spawn (Drop in)
// // //     if (idx == 1)
// // //     {
// // //         player.x = 3 * TILE_WORLD;
// // //         player.y = 2 * TILE_WORLD;
// // //     }

// // //     player.vx = 0;
// // //     player.vy = 0;
// // //     player.hp = 3;
// // //     checkpointX = player.x;
// // //     checkpointY = player.y;
// // //     cameraX = player.x;
// // //     cameraY = player.y;

// // //     if (idx > saveData.highestLevel)
// // //         saveData.highestLevel = idx;
// // //     saveGame();
// // // }

// // // void triggerTransition(int nextIdx)
// // // {
// // //     fadingOut = true;
// // //     nextLevelTarget = nextIdx;
// // // }

// // // void respawn()
// // // {
// // //     saveData.totalDeaths++;
// // //     player.x = checkpointX;
// // //     player.y = checkpointY;
// // //     player.vx = 0;
// // //     player.vy = 0;
// // //     player.hp = 3;
// // //     currentContext = MapContext::Main;
// // //     activeHidden = nullptr;
// // //     spawnParticles(player.x, player.y, 15, {255, 50, 50, 255});
// // //     fadeAlpha = 255.0f;
// // //     fadingOut = false;
// // //     cameraShakeTimer = 0.5f;
// // // }

// // // void damageEnemy(Enemy &e)
// // // {
// // //     e.hp--;
// // //     if (e.hp <= 0)
// // //     {
// // //         e.active = false;
// // //         spawnParticles(e.x, e.y, 10, {255, 100, 0, 255});
// // //     }
// // //     else
// // //         spawnParticles(e.x, e.y, 5, {200, 200, 200, 255});
// // // }

// // // void damagePlayer()
// // // {
// // //     if (player.dashTimer > 0)
// // //         return;
// // //     player.hp--;
// // //     cameraShakeTimer = 0.3f;
// // //     spawnParticles(player.x, player.y, 10, {255, 0, 0, 255});
// // //     if (player.hp <= 0)
// // //         respawn();
// // // }

// // // void updatePhysics(float dt)
// // // {
// // //     const Uint8 *k = SDL_GetKeyboardState(nullptr);

// // //     if (player.dashCooldown > 0)
// // //         player.dashCooldown -= dt;
// // //     if (player.attackTimer > 0)
// // //         player.attackTimer -= dt;
// // //     if (player.meleeVisualTimer > 0)
// // //         player.meleeVisualTimer -= dt;
// // //     if (player.shootTimer > 0)
// // //         player.shootTimer -= dt;

// // //     if (player.isDashing)
// // //     {
// // //         player.dashTimer -= dt;
// // //         if (player.dashTimer <= 0)
// // //         {
// // //             player.isDashing = false;
// // //             player.vx = 0;
// // //         }
// // //         else
// // //         {
// // //             spawnParticles(player.x + player.w / 2, player.y + player.h / 2, 1, {100, 200, 255, 100});
// // //             return;
// // //         }
// // //     }

// // //     float speed = MOVE_SPEED;
// // //     player.isCrouching = k[SDL_SCANCODE_S] || k[SDL_SCANCODE_DOWN];
// // //     player.h = player.isCrouching ? TILE_WORLD / 2 : TILE_WORLD;
// // //     if (player.isCrouching)
// // //         speed *= 0.5f;

// // //     player.vx = 0;

// // //     if (k[SDL_SCANCODE_LEFT])
// // //     {
// // //         player.vx = -speed;
// // //         player.facingRight = false;
// // //     }
// // //     if (k[SDL_SCANCODE_RIGHT])
// // //     {
// // //         player.vx = speed;
// // //         player.facingRight = true;
// // //     }
// // //     player.vy += GRAVITY;

// // //     Level &lvl = levels[currentLevelIndex];
// // //     auto &tiles = (currentContext == MapContext::Main) ? lvl.tiles : activeHidden->tiles;

// // //     for (auto &t : tiles)
// // //     {
// // //         if (!t.active)
// // //             continue;
// // //         if (t.type == TileType::MovingPlatform)
// // //         {
// // //             t.x += t.speed * t.dir;
// // //             if (t.x >= t.maxX)
// // //                 t.dir = -1;
// // //             if (t.x <= t.minX)
// // //                 t.dir = 1;
// // //         }
// // //         if (t.type == TileType::FallingPlatform && t.shaking)
// // //         {
// // //             t.fallTimer += dt;
// // //             t.x = t.originalX + (rand() % 3 - 1);
// // //             if (t.fallTimer > 0.5f)
// // //             {
// // //                 t.falling = true;
// // //                 t.shaking = false;
// // //             }
// // //         }
// // //         if (t.type == TileType::FallingPlatform && t.falling)
// // //         {
// // //             t.y += 5.0f;
// // //             if (t.y > lvl.tiles.back().y + 500)
// // //                 t.active = false;
// // //         }
// // //     }

// // //     player.x += player.vx;
// // //     SDL_Rect pRect = getRect(player.x, player.y, player.w, player.h);
// // //     for (auto &t : tiles)
// // //     {
// // //         if (!t.active || t.type == TileType::Key || t.type == TileType::Checkpoint || t.type == TileType::Spike || t.type == TileType::Fire || t.type == TileType::Spring || t.type == TileType::LevelEnd)
// // //             continue;
// // //         if (t.type == TileType::FallingPlatform && t.falling)
// // //             continue;
// // //         SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);
// // //         if (checkCollision(pRect, tRect))
// // //         {
// // //             if (player.vx > 0)
// // //                 player.x = tRect.x - player.w;
// // //             if (player.vx < 0)
// // //                 player.x = tRect.x + tRect.w;
// // //             player.vx = 0;
// // //         }
// // //     }

// // //     player.y += player.vy;
// // //     player.onGround = false;
// // //     pRect = getRect(player.x, player.y, player.w, player.h);
// // //     for (auto &t : tiles)
// // //     {
// // //         if (!t.active)
// // //             continue;
// // //         SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);

// // //         if (t.type == TileType::Key)
// // //         {
// // //             if (checkCollision(pRect, tRect))
// // //             {
// // //                 t.active = false;
// // //                 keysCollected++;
// // //                 spawnParticles(t.x, t.y, 10, {255, 215, 0, 255});
// // //             }
// // //             continue;
// // //         }
// // //         if (t.type == TileType::Spring)
// // //         {
// // //             SDL_Rect sRect = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
// // //             if (checkCollision(pRect, sRect) && player.vy > 0)
// // //             {
// // //                 player.vy = -30.0f;
// // //                 player.jumpsLeft = 2;
// // //                 spawnParticles(player.x, player.y + player.h, 8, {50, 255, 50, 255});
// // //             }
// // //             continue;
// // //         }
// // //         if (t.type == TileType::Spike || t.type == TileType::Fire)
// // //         {
// // //             SDL_Rect hurtBox = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
// // //             if (checkCollision(pRect, hurtBox))
// // //                 damagePlayer();
// // //             continue;
// // //         }
// // //         if (t.type == TileType::LevelEnd)
// // //         {
// // //             if (checkCollision(pRect, tRect))
// // //             {
// // //                 if (currentContext == MapContext::Hidden)
// // //                 {
// // //                     currentContext = MapContext::Main;
// // //                     activeHidden = nullptr;
// // //                     player.x = checkpointX;
// // //                     player.y = checkpointY;
// // //                 }
// // //                 else if (keysCollected >= 1)
// // //                     triggerTransition(currentLevelIndex + 1);
// // //             }
// // //             continue;
// // //         }
// // //         if (t.type == TileType::Checkpoint)
// // //         {
// // //             if (checkCollision(pRect, tRect))
// // //             {
// // //                 checkpointX = t.x;
// // //                 checkpointY = t.y;
// // //             }
// // //             continue;
// // //         }

// // //         if (t.type == TileType::MovingPlatform)
// // //         {
// // //             if (player.x + player.w > t.x && player.x < t.x + TILE_WORLD && player.y + player.h >= t.y - 4 && player.y + player.h <= t.y + 10 && player.vy >= 0)
// // //             {
// // //                 player.y = t.y - player.h;
// // //                 player.vy = 0;
// // //                 player.onGround = true;
// // //                 player.jumpsLeft = 2;
// // //                 player.x += t.speed * t.dir;
// // //                 continue;
// // //             }
// // //         }
// // //         if (t.type == TileType::FallingPlatform && t.falling)
// // //             continue;

// // //         if (checkCollision(pRect, tRect))
// // //         {
// // //             if (t.type == TileType::FallingPlatform && player.vy > 0)
// // //                 t.shaking = true;
// // //             if (player.vy > 0)
// // //             {
// // //                 player.y = tRect.y - player.h;
// // //                 player.vy = 0;
// // //                 player.onGround = true;
// // //                 player.jumpsLeft = 2;
// // //             }
// // //             else if (player.vy < 0)
// // //             {
// // //                 player.y = tRect.y + tRect.h;
// // //                 player.vy = 0;
// // //                 if (t.type == TileType::Breakable && !lvl.hiddenAreas.empty())
// // //                 {
// // //                     currentContext = MapContext::Hidden;
// // //                     activeHidden = &lvl.hiddenAreas[0];
// // //                     player.x = activeHidden->bounds.x + 64;
// // //                     player.y = activeHidden->bounds.y + 64;
// // //                 }
// // //             }
// // //         }
// // //     }

// // //     if (player.y > 2000)
// // //         respawn();

// // //     auto &enemies = (currentContext == MapContext::Main) ? lvl.enemies : activeHidden->enemies;
// // //     pRect = getRect(player.x, player.y, player.w, player.h);

// // //     if (player.meleeVisualTimer > 0)
// // //     {
// // //         SDL_Rect attackBox = {(int)(player.x + (player.facingRight ? player.w : -MELEE_RANGE)), (int)player.y, (int)MELEE_RANGE, player.h};
// // //         for (auto &e : enemies)
// // //         {
// // //             if (!e.active)
// // //                 continue;
// // //             if (checkCollision(attackBox, getRect(e.x, e.y, e.w, e.h)))
// // //             {
// // //                 bool blocked = false;
// // //                 if (e.type == TileType::EnemyShield)
// // //                 {
// // //                     bool hitFromLeft = player.x < e.x;
// // //                     bool enemyFacingLeft = e.vx < 0;
// // //                     if (hitFromLeft == enemyFacingLeft)
// // //                         blocked = true;
// // //                 }
// // //                 if (blocked)
// // //                     spawnParticles(e.x, e.y, 2, {100, 100, 255, 255});
// // //                 else
// // //                     damageEnemy(e);
// // //             }
// // //         }
// // //         if (lvl.boss.active && checkCollision(attackBox, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
// // //         {
// // //             lvl.boss.hp--;
// // //             spawnParticles(lvl.boss.x + lvl.boss.w / 2, lvl.boss.y + lvl.boss.h / 2, 5, {255, 0, 0, 255});
// // //             if (lvl.boss.hp <= 0)
// // //             {
// // //                 lvl.boss.active = false;
// // //                 triggerTransition(currentLevelIndex + 1);
// // //             }
// // //         }
// // //     }

// // //     for (auto &e : enemies)
// // //     {
// // //         if (!e.active)
// // //             continue;
// // //         SDL_Rect eRect = getRect(e.x, e.y, e.w, e.h);

// // //         if (e.type == TileType::EnemyPatrol || e.type == TileType::EnemyShield)
// // //         {
// // //             if (e.vx == 0)
// // //                 e.vx = ENEMY_SPEED;
// // //             e.x += e.vx;
// // //             if (e.x > e.startX + 64)
// // //                 e.vx = -ENEMY_SPEED;
// // //             if (e.x < e.startX - 64)
// // //                 e.vx = ENEMY_SPEED;
// // //             if (checkCollision(pRect, eRect))
// // //                 damagePlayer();
// // //         }
// // //         else if (e.type == TileType::EnemyFly)
// // //         {
// // //             float dist = std::sqrt(std::pow(player.x - e.x, 2) + std::pow(player.y - e.y, 2));
// // //             if (dist < 300.0f)
// // //             {
// // //                 float angle = std::atan2(player.y - e.y, player.x - e.x);
// // //                 e.x += std::cos(angle) * FLY_SPEED * 0.5f;
// // //                 e.y += std::sin(angle) * FLY_SPEED * 0.5f;
// // //             }
// // //             if (checkCollision(pRect, eRect))
// // //                 damagePlayer();
// // //         }
// // //         else if (e.type == TileType::Turret)
// // //         {
// // //             e.timer += dt;
// // //             if (e.timer > TURRET_RATE)
// // //             {
// // //                 e.timer = 0;
// // //                 Projectile p;
// // //                 p.x = e.x;
// // //                 p.y = e.y + 8;
// // //                 p.vx = (player.x < e.x) ? -4.0f : 4.0f;
// // //                 p.vy = 0;
// // //                 p.active = true;
// // //                 p.fromPlayer = false;
// // //                 projectiles.push_back(p);
// // //             }
// // //         }
// // //     }

// // //     if (lvl.boss.active)
// // //     {
// // //         Boss &b = lvl.boss;
// // //         SDL_Rect bRect = getRect(b.x, b.y, b.w, b.h);
// // //         if (checkCollision(pRect, bRect))
// // //             damagePlayer();

// // //         b.stateTimer += dt;
// // //         switch (b.state)
// // //         {
// // //         case BossState::Idle:
// // //             if (b.stateTimer > 2.0f)
// // //             {
// // //                 b.state = BossState::Charge;
// // //                 b.stateTimer = 0;
// // //             }
// // //             break;
// // //         case BossState::Charge:
// // //             b.x += (player.x < b.x ? -BOSS_SPEED : BOSS_SPEED);
// // //             if (b.stateTimer > 2.0f)
// // //             {
// // //                 b.state = BossState::Shoot;
// // //                 b.stateTimer = 0;
// // //             }
// // //             break;
// // //         case BossState::Shoot:
// // //             if ((int)(b.stateTimer * 10) % 5 == 0)
// // //             {
// // //                 Projectile p;
// // //                 p.x = b.x + b.w / 2;
// // //                 p.y = b.y + b.h / 2;
// // //                 p.vx = (rand() % 10 - 5);
// // //                 p.vy = (rand() % 10 - 5);
// // //                 p.active = true;
// // //                 p.fromPlayer = false;
// // //                 projectiles.push_back(p);
// // //             }
// // //             if (b.stateTimer > 2.0f)
// // //             {
// // //                 b.state = BossState::Idle;
// // //                 b.stateTimer = 0;
// // //             }
// // //             break;
// // //         }
// // //     }

// // //     for (auto &p : projectiles)
// // //     {
// // //         if (!p.active)
// // //             continue;
// // //         p.x += p.vx;
// // //         p.y += p.vy;
// // //         SDL_Rect r = getRect(p.x, p.y, 8, 8);
// // //         if (p.fromPlayer)
// // //         {
// // //             for (auto &e : enemies)
// // //             {
// // //                 if (e.active && checkCollision(r, getRect(e.x, e.y, e.w, e.h)))
// // //                 {
// // //                     if (e.type != TileType::EnemyShield)
// // //                         damageEnemy(e);
// // //                     p.active = false;
// // //                 }
// // //             }
// // //             if (lvl.boss.active && checkCollision(r, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
// // //             {
// // //                 lvl.boss.hp--;
// // //                 p.active = false;
// // //             }
// // //         }
// // //         else
// // //         {
// // //             if (checkCollision(r, pRect))
// // //             {
// // //                 damagePlayer();
// // //                 p.active = false;
// // //             }
// // //         }
// // //         if (std::abs(p.x - player.x) > 600)
// // //             p.active = false;
// // //     }

// // //     for (auto it = particles.begin(); it != particles.end();)
// // //     {
// // //         it->x += it->vx;
// // //         it->y += it->vy;
// // //         it->life -= 0.05f;
// // //         if (it->life <= 0)
// // //             it = particles.erase(it);
// // //         else
// // //             ++it;
// // //     }
// // // }

// // // /* ---------------------------------------------------
// // //    MAIN
// // // --------------------------------------------------- */
// // // int main(int argc, char *argv[])
// // // {
// // //     SDL_Init(SDL_INIT_VIDEO);
// // //     IMG_Init(IMG_INIT_PNG);
// // //     TTF_Init();
// // //     SDL_Window *window = SDL_CreateWindow("Reflex Rift - Mario Recreations", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
// // //     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

// // //     // CHANGE PATH IF NEEDED
// // //     std::string path = "/Users/lazycodebaker/Documents/Dev/CPP/platformer_2d_temp/assets/";
// // //     SDL_Texture *tilesTex = IMG_LoadTexture(renderer, (path + "spritesheet_tiles_resized.png").c_str());
// // //     SDL_Texture *playerTex = IMG_LoadTexture(renderer, (path + "spritesheet_characters.png").c_str());
// // //     TTF_Font *font = TTF_OpenFont((path + "PixelifySans-VariableFont_wght.ttf").c_str(), 24);

// // //     // --- GLOW TEXTURE GENERATION ---
// // //     // Create a 64x64 texture for the glow lights (Gold/Green)
// // //     SDL_Texture *glowTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 64);
// // //     SDL_SetTextureBlendMode(glowTex, SDL_BLENDMODE_ADD);
// // //     SDL_SetRenderTarget(renderer, glowTex);
// // //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
// // //     SDL_RenderClear(renderer);
// // //     // Draw a radial gradient manually
// // //     for (int y = 0; y < 64; y++)
// // //     {
// // //         for (int x = 0; x < 64; x++)
// // //         {
// // //             float dist = std::sqrt(std::pow(x - 32, 2) + std::pow(y - 32, 2));
// // //             if (dist < 32)
// // //             {
// // //                 int alpha = (int)(255 * (1.0f - dist / 32.0f));
// // //                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
// // //                 SDL_RenderDrawPoint(renderer, x, y);
// // //             }
// // //         }
// // //     }
// // //     SDL_SetRenderTarget(renderer, nullptr);

// // //     // Lighting (Shadow Mask)
// // //     SDL_Texture *lightLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
// // //     SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
// // //     SDL_Texture *lightMask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 300);
// // //     SDL_BlendMode eraser = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
// // //     SDL_SetTextureBlendMode(lightMask, eraser);
// // //     SDL_SetRenderTarget(renderer, lightMask);
// // //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
// // //     SDL_RenderClear(renderer);
// // //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
// // //     for (int w = 0; w < 300; w++)
// // //         for (int h = 0; h < 300; h++)
// // //             if ((150 - w) * (150 - w) + (150 - h) * (150 - h) < 22500)
// // //                 SDL_RenderDrawPoint(renderer, w, h);

// // //     SDL_SetRenderTarget(renderer, nullptr);

// // //     loadSave();
// // //     levels = parseMap(path + "map.json");

// // //     if (levels.empty())
// // //     {
// // //         std::cerr << "CRITICAL ERROR: Could not load levels! Check map.json path." << std::endl;

// // //         // Create a dummy level so the game doesn't crash or auto-win
// // //         Level errorLevel;
// // //         errorLevel.name = "ERROR: MAP NOT FOUND";
// // //         errorLevel.boss.active = false;
// // //         errorLevel.totalKeys = 0;
// // //         levels.push_back(errorLevel);
// // //     }

// // //     auto renderTextCentered = [&](std::string text, int y, SDL_Color color, float scale = 1.0f)
// // //     {
// // //         if (!font)
// // //             return;
// // //         SDL_Surface *s = TTF_RenderText_Blended(font, text.c_str(), color);
// // //         if (!s)
// // //             return;
// // //         SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
// // //         int w = (int)(s->w * scale);
// // //         int h = (int)(s->h * scale);
// // //         SDL_Rect dst = {WINDOW_WIDTH / 2 - w / 2, y, w, h};
// // //         SDL_RenderCopy(renderer, t, nullptr, &dst);
// // //         SDL_FreeSurface(s);
// // //         SDL_DestroyTexture(t);
// // //     };

// // //     bool running = true;
// // //     Uint32 lastTime = SDL_GetTicks();

// // //     while (running)
// // //     {
// // //         Uint32 currentTime = SDL_GetTicks();
// // //         float dt = (currentTime - lastTime) / 1000.0f;
// // //         lastTime = currentTime;

// // //         SDL_Event e;
// // //         while (SDL_PollEvent(&e))
// // //         {
// // //             if (e.type == SDL_QUIT)
// // //                 running = false;
// // //             if (e.type == SDL_KEYDOWN)
// // //             {
// // //                 if (currentState == GameState::Menu)
// // //                 {
// // //                     if (saveData.gameCompleted)
// // //                     {
// // //                         if (e.key.keysym.scancode == SDL_SCANCODE_UP)
// // //                         {
// // //                             menuSelection--;
// // //                             if (menuSelection < 0)
// // //                                 menuSelection = levels.size() - 1;
// // //                         }
// // //                         if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
// // //                         {
// // //                             menuSelection++;
// // //                             if (menuSelection >= levels.size())
// // //                                 menuSelection = 0;
// // //                         }
// // //                         if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// // //                         {
// // //                             currentState = GameState::Playing;
// // //                             resetLevel(menuSelection);
// // //                         }
// // //                     }
// // //                     else
// // //                     {
// // //                         if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// // //                         {
// // //                             currentState = GameState::Playing;
// // //                             resetLevel((saveData.highestLevel < levels.size()) ? saveData.highestLevel : 0);
// // //                         }
// // //                     }
// // //                     if (e.key.keysym.scancode == SDL_SCANCODE_L)
// // //                         enableLighting = !enableLighting;
// // //                 }
// // //                 else if (currentState == GameState::Win)
// // //                 {
// // //                     if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// // //                         currentState = GameState::Menu;
// // //                 }
// // //                 else if (currentState == GameState::Playing)
// // //                 {
// // //                     if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
// // //                         currentState = GameState::Menu;
// // //                     if (e.key.keysym.scancode == SDL_SCANCODE_SPACE && player.jumpsLeft > 0)
// // //                     {
// // //                         player.vy = JUMP_FORCE;
// // //                         player.jumpsLeft--;
// // //                         player.onGround = false;
// // //                     }
// // //                     if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT && !player.isDashing && player.dashCooldown <= 0)
// // //                     {
// // //                         player.isDashing = true;
// // //                         player.dashTimer = DASH_DURATION;
// // //                         player.dashCooldown = DASH_COOLDOWN;
// // //                         player.vx = player.facingRight ? DASH_SPEED : -DASH_SPEED;
// // //                         player.vy = 0;
// // //                     }
// // //                     if (e.key.keysym.scancode == SDL_SCANCODE_Z && player.attackTimer <= 0)
// // //                     {
// // //                         player.attackTimer = MELEE_COOLDOWN;
// // //                         player.meleeVisualTimer = 0.1f;
// // //                     }
// // //                     if (e.key.keysym.scancode == SDL_SCANCODE_X && player.shootTimer <= 0)
// // //                     {
// // //                         player.shootTimer = SHOOT_COOLDOWN;
// // //                         Projectile p;
// // //                         p.x = player.x + player.w / 2;
// // //                         p.y = player.y + player.h / 2;
// // //                         p.vx = player.facingRight ? PROJECTILE_SPEED : -PROJECTILE_SPEED;
// // //                         p.vy = 0;
// // //                         p.active = true;
// // //                         p.fromPlayer = true;
// // //                         projectiles.push_back(p);
// // //                     }
// // //                 }
// // //             }
// // //         }

// // //         if (currentState == GameState::Playing)
// // //         {
// // //             updatePhysics(dt);
// // //             if (fadingOut)
// // //             {
// // //                 fadeAlpha += FADE_SPEED * dt;
// // //                 if (fadeAlpha >= 255)
// // //                 {
// // //                     resetLevel(nextLevelTarget);
// // //                     fadingOut = false;
// // //                 }
// // //             }
// // //             else if (fadeAlpha > 0)
// // //                 fadeAlpha -= FADE_SPEED * dt;

// // //             if (cameraShakeTimer > 0)
// // //             {
// // //                 cameraShakeTimer -= dt;
// // //                 cameraX += (rand() % 10 - 5);
// // //                 cameraY += (rand() % 10 - 5);
// // //             }
// // //         }

// // //         float targetCamX = player.x - WINDOW_WIDTH / 2;
// // //         float targetCamY = player.y - WINDOW_HEIGHT / 2;
// // //         cameraX += (targetCamX - cameraX) * 0.1f;
// // //         cameraY += (targetCamY - cameraY) * 0.1f;

// // //         SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
// // //         SDL_RenderClear(renderer);

// // //         if (currentState == GameState::Menu)
// // //         {
// // //             int y = 50;
// // //             renderTextCentered("Reflex Rift", y, {255, 255, 255, 255}, 1.5f);
// // //             y += 50;
// // //             renderTextCentered("--- CONTROLS ---", y, {180, 180, 180, 255}, 0.8f);
// // //             y += 25;
// // //             renderTextCentered("Arrows: Move | Space: Jump | Shift: Dash", y, {180, 180, 180, 255}, 0.8f);
// // //             y += 25;
// // //             renderTextCentered("Z: Melee | X: Fireball", y, {180, 180, 180, 255}, 0.8f);
// // //             y += 25;
// // //             renderTextCentered("L: Toggle Lights", y, {180, 180, 180, 255}, 0.8f);
// // //             y += 40;

// // //             if (saveData.gameCompleted)
// // //             {
// // //                 renderTextCentered("GAME COMPLETED! SELECT LEVEL:", y, {255, 215, 0, 255});
// // //                 y += 40;
// // //                 for (int i = 0; i < levels.size(); i++)
// // //                 {
// // //                     std::string lvlName = "Level " + std::to_string(i + 1) + ": " + levels[i].name;
// // //                     SDL_Color c = (i == menuSelection) ? SDL_Color{100, 255, 100, 255} : SDL_Color{180, 180, 180, 255};
// // //                     if (i == menuSelection)
// // //                         lvlName = "> " + lvlName + " <";
// // //                     renderTextCentered(lvlName, y, c);
// // //                     y += 30;
// // //                 }
// // //             }
// // //             else
// // //             {
// // //                 renderTextCentered("Press ENTER to Start", y, {255, 215, 0, 255});
// // //             }
// // //         }
// // //         else if (currentState == GameState::Playing)
// // //         {
// // //             // Sky Background
// // //             SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255); // SMB Sky Blue
// // //             SDL_RenderClear(renderer);

// // //             Level &level = levels[currentLevelIndex];
// // //             auto &tiles = (currentContext == MapContext::Main) ? level.tiles : activeHidden->tiles;
// // //             auto &enemies = (currentContext == MapContext::Main) ? level.enemies : activeHidden->enemies;

// // //             // Draw Tiles
// // //             for (auto &t : tiles)
// // //             {
// // //                 if (!t.active)
// // //                     continue;
// // //                 SDL_Rect d = getRect(t.x - cameraX, t.y - cameraY, TILE_WORLD, TILE_WORLD);
// // //                 SDL_Rect s = getSrcRect(t.type);
// // //                 SDL_RenderCopy(renderer, tilesTex, &s, &d);
// // //             }

// // //             // --- DRAW LIGHT GLOWS (NEW) ---
// // //             for (auto &t : tiles)
// // //             {
// // //                 if (!t.active)
// // //                     continue;
// // //                 if (t.type == TileType::Key)
// // //                 {
// // //                     SDL_Rect d = getRect(t.x - cameraX - 16, t.y - cameraY - 16, 64, 64);
// // //                     SDL_SetTextureColorMod(glowTex, 255, 215, 0); // GOLD
// // //                     SDL_RenderCopy(renderer, glowTex, nullptr, &d);
// // //                 }
// // //                 else if (t.type == TileType::LevelEnd)
// // //                 {
// // //                     SDL_Rect d = getRect(t.x - cameraX - 16, t.y - cameraY - 16, 64, 64);
// // //                     SDL_SetTextureColorMod(glowTex, 0, 255, 0); // GREEN
// // //                     SDL_RenderCopy(renderer, glowTex, nullptr, &d);
// // //                 }
// // //             }

// // //             for (auto &e : enemies)
// // //             {
// // //                 if (!e.active)
// // //                     continue;
// // //                 SDL_Rect d = getRect(e.x - cameraX, e.y - cameraY, e.w, e.h);
// // //                 SDL_Rect s = getSrcRect(e.type);
// // //                 SDL_RenderCopyEx(renderer, tilesTex, &s, &d, 0, nullptr, (e.vx > 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
// // //             }
// // //             if (level.boss.active)
// // //             {
// // //                 SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
// // //                 SDL_Rect bRect = getRect(level.boss.x - cameraX, level.boss.y - cameraY, level.boss.w, level.boss.h);
// // //                 SDL_RenderFillRect(renderer, &bRect);
// // //             }

// // //             SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
// // //             for (auto &p : projectiles)
// // //             {
// // //                 if (!p.active)
// // //                     continue;
// // //                 SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 8, 8);
// // //                 SDL_RenderFillRect(renderer, &r);
// // //             }
// // //             for (auto &p : particles)
// // //             {
// // //                 SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, (Uint8)(p.life * 255));
// // //                 SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 4, 4);
// // //                 SDL_RenderFillRect(renderer, &r);
// // //             }

// // //             // Player
// // //             int pRow = (!player.onGround) ? 1 : 0;
// // //             if (SDL_GetTicks() - player.lastAnimTick > ANIM_SPEED_PLAYER)
// // //             {
// // //                 player.animFrame = (player.animFrame + 1) % 2;
// // //                 player.lastAnimTick = SDL_GetTicks();
// // //             }
// // //             SDL_Rect pSrc = {player.animFrame * 16, pRow * 16, 16, 16};
// // //             if (player.isCrouching)
// // //                 pSrc.h = 16;
// // //             SDL_Rect pDst = getRect(player.x - cameraX, player.y - cameraY, player.w, player.h);
// // //             SDL_RenderCopyEx(renderer, playerTex, &pSrc, &pDst, 0, nullptr, player.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

// // //             if (player.meleeVisualTimer > 0)
// // //             {
// // //                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
// // //                 SDL_Rect swing = {(int)(pDst.x + (player.facingRight ? pDst.w : -MELEE_RANGE)), pDst.y, (int)MELEE_RANGE, pDst.h};
// // //                 SDL_RenderFillRect(renderer, &swing);
// // //             }

// // //             // Dark Mode Lighting
// // //             if (enableLighting && level.isDark)
// // //             {
// // //                 SDL_SetRenderTarget(renderer, lightLayer);
// // //                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
// // //                 SDL_RenderClear(renderer);
// // //                 SDL_Rect lDst = {(int)(player.x - cameraX + player.w / 2 - 150), (int)(player.y - cameraY + player.h / 2 - 150), 300, 300};
// // //                 SDL_RenderCopy(renderer, lightMask, nullptr, &lDst);
// // //                 SDL_SetRenderTarget(renderer, nullptr);
// // //                 SDL_RenderCopy(renderer, lightLayer, nullptr, nullptr);
// // //             }

// // //             // HUD
// // //             if (font)
// // //             {
// // //                 std::string hud = "HP: " + std::to_string(player.hp) + " | Key: " + std::to_string(keysCollected) + "/1";
// // //                 SDL_Surface *s = TTF_RenderText_Blended(font, hud.c_str(), {255, 255, 255, 255});
// // //                 SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
// // //                 SDL_Rect d = {20, 20, s->w, s->h};
// // //                 SDL_RenderCopy(renderer, t, nullptr, &d);
// // //                 SDL_FreeSurface(s);
// // //                 SDL_DestroyTexture(t);
// // //             }
// // //             if (fadeAlpha > 0)
// // //             {
// // //                 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
// // //                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)fadeAlpha);
// // //                 SDL_Rect fs = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
// // //                 SDL_RenderFillRect(renderer, &fs);
// // //             }
// // //         }
// // //         else if (currentState == GameState::Win)
// // //         {
// // //             renderTextCentered("ALL LEVELS CLEARED!", WINDOW_HEIGHT / 2 - 30, {255, 215, 0, 255}, 2.0f);
// // //             renderTextCentered("Press ENTER to return to Menu", WINDOW_HEIGHT / 2 + 40, {200, 200, 200, 255}, 1.0f);
// // //         }

// // //         SDL_RenderPresent(renderer);
// // //     }

// // //     SDL_DestroyTexture(glowTex);
// // //     SDL_DestroyTexture(lightLayer);
// // //     SDL_DestroyTexture(lightMask);
// // //     SDL_DestroyTexture(tilesTex);
// // //     SDL_DestroyTexture(playerTex);
// // //     TTF_CloseFont(font);
// // //     SDL_DestroyRenderer(renderer);
// // //     SDL_DestroyWindow(window);
// // //     IMG_Quit();
// // //     TTF_Quit();
// // //     SDL_Quit();
// // //     return 0;
// // // }

// // #include <SDL2/SDL.h>
// // #include <SDL2/SDL_image.h>
// // #include <SDL2/SDL_ttf.h>
// // #include <nlohmann/json.hpp>

// // #include <vector>
// // #include <fstream>
// // #include <iostream>
// // #include <cmath>
// // #include <random>
// // #include <algorithm>

// // using json = nlohmann::json;

// // /* ---------------------------------------------------
// //    CONSTANTS & CONFIG
// // --------------------------------------------------- */
// // constexpr int WINDOW_WIDTH = 1280;
// // constexpr int WINDOW_HEIGHT = 720;

// // constexpr int TILE_SIZE = 16;
// // constexpr int TILE_SCALE = 2;
// // constexpr int TILE_WORLD = TILE_SIZE * TILE_SCALE;

// // constexpr int TILESET_TILE_SIZE = 16;
// // constexpr int TILESET_COLUMNS = 9;

// // // Physics
// // constexpr float GRAVITY = 0.981f;
// // constexpr float MOVE_SPEED = 6.0f;
// // constexpr float JUMP_FORCE = -21.0f;
// // constexpr float DASH_SPEED = 15.0f;
// // constexpr float DASH_DURATION = 0.2f;
// // constexpr float DASH_COOLDOWN = 1.0f;

// // // Combat
// // constexpr float MELEE_RANGE = 40.0f;
// // constexpr float MELEE_COOLDOWN = 0.4f;
// // constexpr float SHOOT_COOLDOWN = 0.6f;
// // constexpr float PROJECTILE_SPEED = 10.0f;

// // // Entity Config
// // constexpr float ENEMY_SPEED = 2.0f;
// // constexpr float FLY_SPEED = 3.0f;
// // constexpr float TURRET_RATE = 2.0f;

// // // Boss Config
// // constexpr int BOSS_HP_MAX = 50;
// // constexpr float BOSS_SPEED = 3.5f;

// // // Visuals
// // constexpr int ANIM_SPEED_PLAYER = 120;
// // constexpr int ANIM_SPEED_FIRE = 150;
// // constexpr float SHAKE_DURATION = 0.5f;
// // constexpr float FADE_SPEED = 1500.0f;

// // /* ---------------------------------------------------
// //    ENUMS
// // --------------------------------------------------- */
// // enum class GameState
// // {
// //     Menu,
// //     Playing,
// //     Transition,
// //     GameOver,
// //     Win
// // };

// // enum class TileType
// // {
// //     Ground,
// //     Spike,
// //     Checkpoint,
// //     Fire,
// //     Key,
// //     Breakable,
// //     LevelEnd,
// //     Spring,
// //     EnemyPatrol,
// //     EnemyFly,
// //     Turret,
// //     MovingPlatform,
// //     FallingPlatform,
// //     EnemyShield,
// //     Boss
// // };

// // enum class MapContext
// // {
// //     Main,
// //     Hidden
// // };
// // enum class PlayerAnim
// // {
// //     Idle,
// //     Run,
// //     Jump,
// //     Crouch,
// //     Dash,
// //     Attack
// // };
// // enum class BossState
// // {
// //     Idle,
// //     Charge,
// //     Shoot
// // };

// // /* ---------------------------------------------------
// //    STRUCTS
// // --------------------------------------------------- */
// // struct Tile
// // {
// //     float x{}, y{};
// //     TileType type{};
// //     bool active{true};
// //     float minX{}, maxX{}, speed{};
// //     int dir{1};
// //     float fallTimer{0.0f};
// //     bool shaking{false}, falling{false};
// //     float originalX{}, originalY{};
// // };

// // struct Particle
// // {
// //     float x, y, vx, vy, life;
// //     SDL_Color color;
// // };

// // struct Projectile
// // {
// //     float x, y, vx, vy;
// //     bool active;
// //     bool fromPlayer;
// // };

// // struct Enemy
// // {
// //     float x, y, vx, vy;
// //     float startX;
// //     int w = TILE_WORLD, h = TILE_WORLD;
// //     TileType type;
// //     bool active = true;
// //     float timer = 0.0f;
// //     int hp = 1;
// //     bool facingRight = false;
// // };

// // struct Boss
// // {
// //     float x, y, vx, vy;
// //     int w = 64, h = 64;
// //     int hp = BOSS_HP_MAX;
// //     BossState state = BossState::Idle;
// //     float stateTimer = 0.0f;
// //     bool active = false;
// // };

// // struct Player
// // {
// //     float x{}, y{};
// //     float vx{}, vy{};
// //     int w{TILE_WORLD}, h{TILE_WORLD};
// //     int hp = 3;

// //     bool onGround{false};
// //     int jumpsLeft{2};
// //     bool facingRight{true};

// //     bool isCrouching{false};
// //     bool isDashing{false};
// //     float dashTimer{0.0f};
// //     float dashCooldown{0.0f};

// //     float attackTimer{0.0f};
// //     float meleeVisualTimer{0.0f};
// //     float shootTimer{0.0f};

// //     PlayerAnim anim{PlayerAnim::Idle};
// //     int animFrame{0};
// //     Uint32 lastAnimTick{0};
// // };

// // struct HiddenArea
// // {
// //     std::vector<Tile> tiles;
// //     std::vector<Enemy> enemies;
// //     SDL_Rect bounds{};
// // };

// // struct Level
// // {
// //     std::string name;
// //     std::vector<Tile> tiles;
// //     std::vector<Enemy> enemies;
// //     std::vector<HiddenArea> hiddenAreas;
// //     Boss boss;
// //     // int totalKeys = 1; // COMMENTED OUT: Keys logic disabled
// //     bool isDark{false};
// // };

// // struct SaveData
// // {
// //     int totalDeaths = 0;
// //     int highestLevel = 0;
// //     bool gameCompleted = false;
// // };

// // /* ---------------------------------------------------
// //    GLOBALS
// // --------------------------------------------------- */
// // GameState currentState = GameState::Menu;
// // std::vector<Level> levels;
// // int currentLevelIndex = 0;
// // MapContext currentContext = MapContext::Main;
// // HiddenArea *activeHidden = nullptr;

// // Player player;
// // std::vector<Particle> particles;
// // std::vector<Projectile> projectiles;
// // SaveData saveData;

// // float cameraX = 0, cameraY = 0;
// // float fadeAlpha = 0.0f;
// // bool fadingOut = false;
// // int nextLevelTarget = -1;
// // float checkpointX = 0, checkpointY = 0;
// // // int keysCollected = 0; // COMMENTED OUT: Keys logic disabled
// // float cameraShakeTimer = 0.0f;

// // // Settings
// // bool enableLighting = true;
// // int menuSelection = 0;

// // /* ---------------------------------------------------
// //    UTILS
// // --------------------------------------------------- */
// // SDL_Rect getRect(float x, float y, int w, int h)
// // {
// //     return {(int)std::round(x), (int)std::round(y), w, h};
// // }

// // bool checkCollision(SDL_Rect a, SDL_Rect b)
// // {
// //     return SDL_HasIntersection(&a, &b);
// // }

// // void spawnParticles(float x, float y, int count, SDL_Color color)
// // {
// //     for (int i = 0; i < count; i++)
// //     {
// //         Particle p;
// //         p.x = x;
// //         p.y = y;
// //         p.vx = (rand() % 10 - 5) * 0.5f;
// //         p.vy = (rand() % 10 - 5) * 0.5f - 1.0f;
// //         p.life = 1.0f;
// //         p.color = color;
// //         particles.push_back(p);
// //     }
// // }

// // TileType parseType(const std::string &name)
// // {
// //     if (name == "Spring")
// //         return TileType::Spring;
// //     if (name == "EnemyPatrol")
// //         return TileType::EnemyPatrol;
// //     if (name == "EnemyFly")
// //         return TileType::EnemyFly;
// //     if (name == "EnemyShield")
// //         return TileType::EnemyShield;
// //     if (name == "Turret")
// //         return TileType::Turret;
// //     if (name == "Boss")
// //         return TileType::Boss;
// //     if (name == "Spikes")
// //         return TileType::Spike;
// //     if (name == "Checkpoints")
// //         return TileType::Checkpoint;
// //     if (name == "Fire")
// //         return TileType::Fire;
// //     if (name == "Keys")
// //         return TileType::Key;
// //     if (name == "Breakable")
// //         return TileType::Breakable;
// //     if (name == "LevelEnd")
// //         return TileType::LevelEnd;
// //     if (name == "MovingPlatforms")
// //         return TileType::MovingPlatform;
// //     if (name == "FallingPlatform")
// //         return TileType::FallingPlatform;
// //     return TileType::Ground;
// // }

// // SDL_Rect getSrcRect(TileType type)
// // {
// //     int idx = 1;
// //     switch (type)
// //     {
// //     case TileType::Spring:
// //         idx = 8;
// //         break;
// //     case TileType::Spike:
// //         idx = 68;
// //         break;
// //     case TileType::Checkpoint:
// //         idx = 50;
// //         break;
// //     case TileType::Fire:
// //         idx = 54 + (SDL_GetTicks() / ANIM_SPEED_FIRE % 2);
// //         break;
// //     case TileType::Key:
// //         idx = 86;
// //         break;
// //     case TileType::Breakable:
// //         idx = 21;
// //         break;
// //     case TileType::LevelEnd:
// //         idx = 52;
// //         break;
// //     case TileType::MovingPlatform:
// //         idx = 13;
// //         break;
// //     case TileType::FallingPlatform:
// //         idx = 14;
// //         break;
// //     case TileType::EnemyPatrol:
// //         idx = 67;
// //         break;
// //     case TileType::EnemyFly:
// //         idx = 79;
// //         break;
// //     case TileType::EnemyShield:
// //         idx = 82;
// //         break;
// //     case TileType::Turret:
// //         idx = 80;
// //         break;
// //     case TileType::Boss:
// //         idx = 88;
// //         break;
// //     default:
// //         idx = 1;
// //         break;
// //     }
// //     return {(idx % TILESET_COLUMNS) * TILESET_TILE_SIZE, (idx / TILESET_COLUMNS) * TILESET_TILE_SIZE, TILESET_TILE_SIZE, TILESET_TILE_SIZE};
// // }

// // void saveGame()
// // {
// //     json j;
// //     j["totalDeaths"] = saveData.totalDeaths;
// //     j["highestLevel"] = saveData.highestLevel;
// //     j["gameCompleted"] = saveData.gameCompleted;
// //     std::ofstream o("savegame.json");
// //     o << j;
// // }

// // void loadSave()
// // {
// //     std::ifstream i("savegame.json");
// //     if (i.is_open())
// //     {
// //         json j;
// //         i >> j;
// //         saveData.totalDeaths = j.value("totalDeaths", 0);
// //         saveData.highestLevel = j.value("highestLevel", 0);
// //         saveData.gameCompleted = j.value("gameCompleted", false);
// //     }
// // }

// // std::vector<Level> parseMap(const std::string &path)
// // {
// //     std::vector<Level> lvls;
// //     std::ifstream f(path);
// //     if (!f.is_open())
// //         return lvls;
// //     json j;
// //     f >> j;

// //     for (auto &lj : j["levels"])
// //     {
// //         Level lvl;
// //         lvl.name = lj["name"];
// //         // lvl.totalKeys = 1; // COMMENTED OUT: Keys logic disabled
// //         if (lj.contains("isDark"))
// //             lvl.isDark = lj["isDark"];

// //         for (auto &layer : lj["layers"])
// //         {
// //             bool isHidden = (layer["name"] == "HiddenArea");
// //             HiddenArea ha;
// //             if (isHidden)
// //             {
// //                 auto b = layer["bounds"];
// //                 ha.bounds = {b["x"].get<int>() * TILE_WORLD, b["y"].get<int>() * TILE_WORLD, b["w"].get<int>() * TILE_WORLD, b["h"].get<int>() * TILE_WORLD};
// //             }

// //             for (auto &t : layer["tiles"])
// //             {
// //                 float x = t["x"].get<float>() * TILE_WORLD;
// //                 float y = t["y"].get<float>() * TILE_WORLD;
// //                 std::string typeStr = t.contains("type") ? t["type"] : (layer["name"] == "Entities" ? "Entity" : "Surface");
// //                 if (!t.contains("type") && layer["name"] != "Surface" && layer["name"] != "Entities" && !isHidden)
// //                     typeStr = layer["name"];

// //                 TileType type = parseType(typeStr);

// //                 if (type == TileType::Boss)
// //                 {
// //                     lvl.boss.active = true;
// //                     lvl.boss.x = x;
// //                     lvl.boss.y = y;
// //                     lvl.boss.hp = BOSS_HP_MAX;
// //                 }
// //                 else if (type == TileType::EnemyPatrol || type == TileType::EnemyFly || type == TileType::EnemyShield || type == TileType::Turret)
// //                 {
// //                     Enemy e{x, y, 0, 0, x, TILE_WORLD, TILE_WORLD, type};
// //                     if (isHidden)
// //                         ha.enemies.push_back(e);
// //                     else
// //                         lvl.enemies.push_back(e);
// //                 }
// //                 else
// //                 {
// //                     Tile tile{x, y, type};
// //                     tile.originalX = x;
// //                     tile.originalY = y;
// //                     if (type == TileType::MovingPlatform)
// //                     {
// //                         tile.minX = t["minX"];
// //                         tile.maxX = t["maxX"];
// //                         tile.speed = t["speed"];
// //                     }
// //                     if (isHidden)
// //                         ha.tiles.push_back(tile);
// //                     else
// //                         lvl.tiles.push_back(tile);
// //                 }
// //             }
// //             if (isHidden)
// //                 lvl.hiddenAreas.push_back(ha);
// //         }
// //         lvls.push_back(lvl);
// //     }
// //     return lvls;
// // }

// // /* ---------------------------------------------------
// //    LOGIC
// // --------------------------------------------------- */
// // void resetLevel(int idx)
// // {
// //     // Safety check against empty levels vector
// //     if (levels.empty())
// //         return;

// //     if (idx >= levels.size())
// //     {
// //         saveData.gameCompleted = true;
// //         saveGame();
// //         currentState = GameState::Win;
// //         return;
// //     }

// //     currentLevelIndex = idx;
// //     currentContext = MapContext::Main;
// //     activeHidden = nullptr;
// //     // keysCollected = 0; // COMMENTED OUT: Keys logic disabled
// //     projectiles.clear();
// //     particles.clear();

// //     Level &lvl = levels[idx];
// //     for (auto &e : lvl.enemies)
// //     {
// //         e.active = true;
// //         e.x = e.startX;
// //         e.hp = 1;
// //     }
// //     lvl.boss.active = (lvl.boss.hp > 0);
// //     if (lvl.boss.active)
// //     {
// //         lvl.boss.hp = BOSS_HP_MAX;
// //         lvl.boss.state = BossState::Idle;
// //     }

// //     for (auto &t : lvl.tiles)
// //     {
// //         if (t.type == TileType::Key)
// //             t.active = true;
// //         if (t.type == TileType::FallingPlatform)
// //         {
// //             t.active = true;
// //             t.y = t.originalY;
// //             t.falling = false;
// //             t.shaking = false;
// //         }
// //     }

// //     // Default Spawn
// //     player.x = 2 * TILE_WORLD;
// //     player.y = 10 * TILE_WORLD;

// //     // Level 2 special spawn (Drop in)
// //     if (idx == 1)
// //     {
// //         player.x = 3 * TILE_WORLD;
// //         player.y = 2 * TILE_WORLD;
// //     }

// //     player.vx = 0;
// //     player.vy = 0;
// //     player.hp = 3;
// //     checkpointX = player.x;
// //     checkpointY = player.y;
// //     cameraX = player.x;
// //     cameraY = player.y;

// //     if (idx > saveData.highestLevel)
// //         saveData.highestLevel = idx;
// //     saveGame();
// // }

// // void triggerTransition(int nextIdx)
// // {
// //     fadingOut = true;
// //     nextLevelTarget = nextIdx;
// // }

// // void respawn()
// // {
// //     saveData.totalDeaths++;
// //     player.x = checkpointX;
// //     player.y = checkpointY;
// //     player.vx = 0;
// //     player.vy = 0;
// //     player.hp = 3;
// //     currentContext = MapContext::Main;
// //     activeHidden = nullptr;
// //     spawnParticles(player.x, player.y, 15, {255, 50, 50, 255});
// //     fadeAlpha = 255.0f;
// //     fadingOut = false;
// //     cameraShakeTimer = 0.5f;
// // }

// // void damageEnemy(Enemy &e)
// // {
// //     e.hp--;
// //     if (e.hp <= 0)
// //     {
// //         e.active = false;
// //         spawnParticles(e.x, e.y, 10, {255, 100, 0, 255});
// //     }
// //     else
// //         spawnParticles(e.x, e.y, 5, {200, 200, 200, 255});
// // }

// // void damagePlayer()
// // {
// //     if (player.dashTimer > 0)
// //         return;
// //     player.hp--;
// //     cameraShakeTimer = 0.3f;
// //     spawnParticles(player.x, player.y, 10, {255, 0, 0, 255});
// //     if (player.hp <= 0)
// //         respawn();
// // }

// // void updatePhysics(float dt)
// // {
// //     const Uint8 *k = SDL_GetKeyboardState(nullptr);

// //     if (player.dashCooldown > 0)
// //         player.dashCooldown -= dt;
// //     if (player.attackTimer > 0)
// //         player.attackTimer -= dt;
// //     if (player.meleeVisualTimer > 0)
// //         player.meleeVisualTimer -= dt;
// //     if (player.shootTimer > 0)
// //         player.shootTimer -= dt;

// //     if (player.isDashing)
// //     {
// //         player.dashTimer -= dt;
// //         if (player.dashTimer <= 0)
// //         {
// //             player.isDashing = false;
// //             player.vx = 0;
// //         }
// //         else
// //         {
// //             spawnParticles(player.x + player.w / 2, player.y + player.h / 2, 1, {100, 200, 255, 100});
// //             return;
// //         }
// //     }

// //     float speed = MOVE_SPEED;
// //     player.isCrouching = k[SDL_SCANCODE_S] || k[SDL_SCANCODE_DOWN];
// //     player.h = player.isCrouching ? TILE_WORLD / 2 : TILE_WORLD;
// //     if (player.isCrouching)
// //         speed *= 0.5f;

// //     player.vx = 0;

// //     if (k[SDL_SCANCODE_LEFT])
// //     {
// //         player.vx = -speed;
// //         player.facingRight = false;
// //     }
// //     if (k[SDL_SCANCODE_RIGHT])
// //     {
// //         player.vx = speed;
// //         player.facingRight = true;
// //     }
// //     player.vy += GRAVITY;

// //     Level &lvl = levels[currentLevelIndex];
// //     auto &tiles = (currentContext == MapContext::Main) ? lvl.tiles : activeHidden->tiles;

// //     for (auto &t : tiles)
// //     {
// //         if (!t.active)
// //             continue;
// //         if (t.type == TileType::MovingPlatform)
// //         {
// //             t.x += t.speed * t.dir;
// //             if (t.x >= t.maxX)
// //                 t.dir = -1;
// //             if (t.x <= t.minX)
// //                 t.dir = 1;
// //         }
// //         if (t.type == TileType::FallingPlatform && t.shaking)
// //         {
// //             t.fallTimer += dt;
// //             t.x = t.originalX + (rand() % 3 - 1);
// //             if (t.fallTimer > 0.5f)
// //             {
// //                 t.falling = true;
// //                 t.shaking = false;
// //             }
// //         }
// //         if (t.type == TileType::FallingPlatform && t.falling)
// //         {
// //             t.y += 5.0f;
// //             if (t.y > lvl.tiles.back().y + 500)
// //                 t.active = false;
// //         }
// //     }

// //     player.x += player.vx;
// //     SDL_Rect pRect = getRect(player.x, player.y, player.w, player.h);
// //     for (auto &t : tiles)
// //     {
// //         if (!t.active || t.type == TileType::Key || t.type == TileType::Checkpoint || t.type == TileType::Spike || t.type == TileType::Fire || t.type == TileType::Spring || t.type == TileType::LevelEnd)
// //             continue;
// //         if (t.type == TileType::FallingPlatform && t.falling)
// //             continue;
// //         SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);
// //         if (checkCollision(pRect, tRect))
// //         {
// //             if (player.vx > 0)
// //                 player.x = tRect.x - player.w;
// //             if (player.vx < 0)
// //                 player.x = tRect.x + tRect.w;
// //             player.vx = 0;
// //         }
// //     }

// //     player.y += player.vy;
// //     player.onGround = false;
// //     pRect = getRect(player.x, player.y, player.w, player.h);
// //     for (auto &t : tiles)
// //     {
// //         if (!t.active)
// //             continue;
// //         SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);

// //         /* --- KEYS COMMENTED OUT ---
// //         if (t.type == TileType::Key)
// //         {
// //             if (checkCollision(pRect, tRect))
// //             {
// //                 t.active = false;
// //                 keysCollected++;
// //                 spawnParticles(t.x, t.y, 10, {255, 215, 0, 255});
// //             }
// //             continue;
// //         }
// //         */

// //         if (t.type == TileType::Spring)
// //         {
// //             SDL_Rect sRect = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
// //             if (checkCollision(pRect, sRect) && player.vy > 0)
// //             {
// //                 player.vy = -30.0f;
// //                 player.jumpsLeft = 2;
// //                 spawnParticles(player.x, player.y + player.h, 8, {50, 255, 50, 255});
// //             }
// //             continue;
// //         }
// //         if (t.type == TileType::Spike || t.type == TileType::Fire)
// //         {
// //             SDL_Rect hurtBox = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
// //             if (checkCollision(pRect, hurtBox))
// //                 damagePlayer();
// //             continue;
// //         }
// //         if (t.type == TileType::LevelEnd)
// //         {
// //             if (checkCollision(pRect, tRect))
// //             {
// //                 if (currentContext == MapContext::Hidden)
// //                 {
// //                     currentContext = MapContext::Main;
// //                     activeHidden = nullptr;
// //                     player.x = checkpointX;
// //                     player.y = checkpointY;
// //                 }
// //                 // CHANGED: Removed key check. Simple door logic.
// //                 // else if (keysCollected >= 1)
// //                 else
// //                 {
// //                     triggerTransition(currentLevelIndex + 1);
// //                 }
// //             }
// //             continue;
// //         }
// //         if (t.type == TileType::Checkpoint)
// //         {
// //             if (checkCollision(pRect, tRect))
// //             {
// //                 checkpointX = t.x;
// //                 checkpointY = t.y;
// //             }
// //             continue;
// //         }

// //         if (t.type == TileType::MovingPlatform)
// //         {
// //             if (player.x + player.w > t.x && player.x < t.x + TILE_WORLD && player.y + player.h >= t.y - 4 && player.y + player.h <= t.y + 10 && player.vy >= 0)
// //             {
// //                 player.y = t.y - player.h;
// //                 player.vy = 0;
// //                 player.onGround = true;
// //                 player.jumpsLeft = 2;
// //                 player.x += t.speed * t.dir;
// //                 continue;
// //             }
// //         }
// //         if (t.type == TileType::FallingPlatform && t.falling)
// //             continue;

// //         if (checkCollision(pRect, tRect))
// //         {
// //             if (t.type == TileType::FallingPlatform && player.vy > 0)
// //                 t.shaking = true;
// //             if (player.vy > 0)
// //             {
// //                 player.y = tRect.y - player.h;
// //                 player.vy = 0;
// //                 player.onGround = true;
// //                 player.jumpsLeft = 2;
// //             }
// //             else if (player.vy < 0)
// //             {
// //                 player.y = tRect.y + tRect.h;
// //                 player.vy = 0;
// //                 if (t.type == TileType::Breakable && !lvl.hiddenAreas.empty())
// //                 {
// //                     currentContext = MapContext::Hidden;
// //                     activeHidden = &lvl.hiddenAreas[0];
// //                     player.x = activeHidden->bounds.x + 64;
// //                     player.y = activeHidden->bounds.y + 64;
// //                 }
// //             }
// //         }
// //     }

// //     if (player.y > 2000)
// //         respawn();

// //     auto &enemies = (currentContext == MapContext::Main) ? lvl.enemies : activeHidden->enemies;
// //     pRect = getRect(player.x, player.y, player.w, player.h);

// //     if (player.meleeVisualTimer > 0)
// //     {
// //         SDL_Rect attackBox = {(int)(player.x + (player.facingRight ? player.w : -MELEE_RANGE)), (int)player.y, (int)MELEE_RANGE, player.h};
// //         for (auto &e : enemies)
// //         {
// //             if (!e.active)
// //                 continue;
// //             if (checkCollision(attackBox, getRect(e.x, e.y, e.w, e.h)))
// //             {
// //                 bool blocked = false;
// //                 if (e.type == TileType::EnemyShield)
// //                 {
// //                     bool hitFromLeft = player.x < e.x;
// //                     bool enemyFacingLeft = e.vx < 0;
// //                     if (hitFromLeft == enemyFacingLeft)
// //                         blocked = true;
// //                 }
// //                 if (blocked)
// //                     spawnParticles(e.x, e.y, 2, {100, 100, 255, 255});
// //                 else
// //                     damageEnemy(e);
// //             }
// //         }
// //         if (lvl.boss.active && checkCollision(attackBox, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
// //         {
// //             lvl.boss.hp--;
// //             spawnParticles(lvl.boss.x + lvl.boss.w / 2, lvl.boss.y + lvl.boss.h / 2, 5, {255, 0, 0, 255});
// //             if (lvl.boss.hp <= 0)
// //             {
// //                 lvl.boss.active = false;
// //                 triggerTransition(currentLevelIndex + 1);
// //             }
// //         }
// //     }

// //     for (auto &e : enemies)
// //     {
// //         if (!e.active)
// //             continue;
// //         SDL_Rect eRect = getRect(e.x, e.y, e.w, e.h);

// //         if (e.type == TileType::EnemyPatrol || e.type == TileType::EnemyShield)
// //         {
// //             if (e.vx == 0)
// //                 e.vx = ENEMY_SPEED;
// //             e.x += e.vx;
// //             if (e.x > e.startX + 64)
// //                 e.vx = -ENEMY_SPEED;
// //             if (e.x < e.startX - 64)
// //                 e.vx = ENEMY_SPEED;
// //             if (checkCollision(pRect, eRect))
// //                 damagePlayer();
// //         }
// //         else if (e.type == TileType::EnemyFly)
// //         {
// //             float dist = std::sqrt(std::pow(player.x - e.x, 2) + std::pow(player.y - e.y, 2));
// //             if (dist < 300.0f)
// //             {
// //                 float angle = std::atan2(player.y - e.y, player.x - e.x);
// //                 e.x += std::cos(angle) * FLY_SPEED * 0.5f;
// //                 e.y += std::sin(angle) * FLY_SPEED * 0.5f;
// //             }
// //             if (checkCollision(pRect, eRect))
// //                 damagePlayer();
// //         }
// //         else if (e.type == TileType::Turret)
// //         {
// //             e.timer += dt;
// //             if (e.timer > TURRET_RATE)
// //             {
// //                 e.timer = 0;
// //                 Projectile p;
// //                 p.x = e.x;
// //                 p.y = e.y + 8;
// //                 p.vx = (player.x < e.x) ? -4.0f : 4.0f;
// //                 p.vy = 0;
// //                 p.active = true;
// //                 p.fromPlayer = false;
// //                 projectiles.push_back(p);
// //             }
// //         }
// //     }

// //     if (lvl.boss.active)
// //     {
// //         Boss &b = lvl.boss;
// //         SDL_Rect bRect = getRect(b.x, b.y, b.w, b.h);
// //         if (checkCollision(pRect, bRect))
// //             damagePlayer();

// //         b.stateTimer += dt;
// //         switch (b.state)
// //         {
// //         case BossState::Idle:
// //             if (b.stateTimer > 2.0f)
// //             {
// //                 b.state = BossState::Charge;
// //                 b.stateTimer = 0;
// //             }
// //             break;
// //         case BossState::Charge:
// //             b.x += (player.x < b.x ? -BOSS_SPEED : BOSS_SPEED);
// //             if (b.stateTimer > 2.0f)
// //             {
// //                 b.state = BossState::Shoot;
// //                 b.stateTimer = 0;
// //             }
// //             break;
// //         case BossState::Shoot:
// //             if ((int)(b.stateTimer * 10) % 5 == 0)
// //             {
// //                 Projectile p;
// //                 p.x = b.x + b.w / 2;
// //                 p.y = b.y + b.h / 2;
// //                 p.vx = (rand() % 10 - 5);
// //                 p.vy = (rand() % 10 - 5);
// //                 p.active = true;
// //                 p.fromPlayer = false;
// //                 projectiles.push_back(p);
// //             }
// //             if (b.stateTimer > 2.0f)
// //             {
// //                 b.state = BossState::Idle;
// //                 b.stateTimer = 0;
// //             }
// //             break;
// //         }
// //     }

// //     for (auto &p : projectiles)
// //     {
// //         if (!p.active)
// //             continue;
// //         p.x += p.vx;
// //         p.y += p.vy;
// //         SDL_Rect r = getRect(p.x, p.y, 8, 8);
// //         if (p.fromPlayer)
// //         {
// //             for (auto &e : enemies)
// //             {
// //                 if (e.active && checkCollision(r, getRect(e.x, e.y, e.w, e.h)))
// //                 {
// //                     if (e.type != TileType::EnemyShield)
// //                         damageEnemy(e);
// //                     p.active = false;
// //                 }
// //             }
// //             if (lvl.boss.active && checkCollision(r, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
// //             {
// //                 lvl.boss.hp--;
// //                 p.active = false;
// //             }
// //         }
// //         else
// //         {
// //             if (checkCollision(r, pRect))
// //             {
// //                 damagePlayer();
// //                 p.active = false;
// //             }
// //         }
// //         if (std::abs(p.x - player.x) > 600)
// //             p.active = false;
// //     }

// //     for (auto it = particles.begin(); it != particles.end();)
// //     {
// //         it->x += it->vx;
// //         it->y += it->vy;
// //         it->life -= 0.05f;
// //         if (it->life <= 0)
// //             it = particles.erase(it);
// //         else
// //             ++it;
// //     }
// // }

// // /* ---------------------------------------------------
// //    MAIN
// // --------------------------------------------------- */
// // int main(int argc, char *argv[])
// // {
// //     SDL_Init(SDL_INIT_VIDEO);
// //     IMG_Init(IMG_INIT_PNG);
// //     TTF_Init();
// //     SDL_Window *window = SDL_CreateWindow("Reflex Rift - Mario Recreations", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
// //     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

// //     // CHANGE PATH IF NEEDED
// //     std::string path = "/Users/lazycodebaker/Documents/Dev/CPP/platformer_2d_temp/assets/";
// //     SDL_Texture *tilesTex = IMG_LoadTexture(renderer, (path + "spritesheet_tiles_resized.png").c_str());
// //     SDL_Texture *playerTex = IMG_LoadTexture(renderer, (path + "spritesheet_characters.png").c_str());
// //     TTF_Font *font = TTF_OpenFont((path + "PixelifySans-VariableFont_wght.ttf").c_str(), 24);

// //     // --- GLOW TEXTURE GENERATION ---
// //     // Create a 64x64 texture for the glow lights (Gold/Green)
// //     SDL_Texture *glowTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 64);
// //     SDL_SetTextureBlendMode(glowTex, SDL_BLENDMODE_ADD);
// //     SDL_SetRenderTarget(renderer, glowTex);
// //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
// //     SDL_RenderClear(renderer);
// //     // Draw a radial gradient manually
// //     for (int y = 0; y < 64; y++)
// //     {
// //         for (int x = 0; x < 64; x++)
// //         {
// //             float dist = std::sqrt(std::pow(x - 32, 2) + std::pow(y - 32, 2));
// //             if (dist < 32)
// //             {
// //                 int alpha = (int)(255 * (1.0f - dist / 32.0f));
// //                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
// //                 SDL_RenderDrawPoint(renderer, x, y);
// //             }
// //         }
// //     }
// //     SDL_SetRenderTarget(renderer, nullptr);

// //     // Lighting (Shadow Mask)
// //     SDL_Texture *lightLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
// //     SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
// //     SDL_Texture *lightMask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 300);
// //     SDL_BlendMode eraser = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
// //     SDL_SetTextureBlendMode(lightMask, eraser);
// //     SDL_SetRenderTarget(renderer, lightMask);
// //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
// //     SDL_RenderClear(renderer);
// //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
// //     for (int w = 0; w < 300; w++)
// //         for (int h = 0; h < 300; h++)
// //             if ((150 - w) * (150 - w) + (150 - h) * (150 - h) < 22500)
// //                 SDL_RenderDrawPoint(renderer, w, h);

// //     SDL_SetRenderTarget(renderer, nullptr);

// //     loadSave();
// //     // FIXED: Uses correct path now
// //     levels = parseMap(path + "map.json");

// //     // ADDED SAFETY CHECK for empty map
// //     if (levels.empty())
// //     {
// //         std::cerr << "CRITICAL ERROR: Could not load levels! Check map.json path." << std::endl;
// //         Level errorLevel;
// //         errorLevel.name = "ERROR: MAP NOT FOUND";
// //         errorLevel.boss.active = false;
// //         levels.push_back(errorLevel);
// //     }

// //     auto renderTextCentered = [&](std::string text, int y, SDL_Color color, float scale = 1.0f)
// //     {
// //         if (!font)
// //             return;
// //         SDL_Surface *s = TTF_RenderText_Blended(font, text.c_str(), color);
// //         if (!s)
// //             return;
// //         SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
// //         int w = (int)(s->w * scale);
// //         int h = (int)(s->h * scale);
// //         SDL_Rect dst = {WINDOW_WIDTH / 2 - w / 2, y, w, h};
// //         SDL_RenderCopy(renderer, t, nullptr, &dst);
// //         SDL_FreeSurface(s);
// //         SDL_DestroyTexture(t);
// //     };

// //     bool running = true;
// //     Uint32 lastTime = SDL_GetTicks();

// //     while (running)
// //     {
// //         Uint32 currentTime = SDL_GetTicks();
// //         float dt = (currentTime - lastTime) / 1000.0f;
// //         lastTime = currentTime;

// //         SDL_Event e;
// //         while (SDL_PollEvent(&e))
// //         {
// //             if (e.type == SDL_QUIT)
// //                 running = false;
// //             if (e.type == SDL_KEYDOWN)
// //             {
// //                 if (currentState == GameState::Menu)
// //                 {
// //                     if (saveData.gameCompleted)
// //                     {
// //                         if (e.key.keysym.scancode == SDL_SCANCODE_UP)
// //                         {
// //                             menuSelection--;
// //                             if (menuSelection < 0)
// //                                 menuSelection = levels.size() - 1;
// //                         }
// //                         if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
// //                         {
// //                             menuSelection++;
// //                             if (menuSelection >= levels.size())
// //                                 menuSelection = 0;
// //                         }
// //                         if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// //                         {
// //                             currentState = GameState::Playing;
// //                             resetLevel(menuSelection);
// //                         }
// //                     }
// //                     else
// //                     {
// //                         if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// //                         {
// //                             currentState = GameState::Playing;
// //                             resetLevel((saveData.highestLevel < levels.size()) ? saveData.highestLevel : 0);
// //                         }
// //                     }
// //                     if (e.key.keysym.scancode == SDL_SCANCODE_L)
// //                         enableLighting = !enableLighting;
// //                 }
// //                 else if (currentState == GameState::Win)
// //                 {
// //                     if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
// //                         currentState = GameState::Menu;
// //                 }
// //                 else if (currentState == GameState::Playing)
// //                 {
// //                     if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
// //                         currentState = GameState::Menu;
// //                     if (e.key.keysym.scancode == SDL_SCANCODE_SPACE && player.jumpsLeft > 0)
// //                     {
// //                         player.vy = JUMP_FORCE;
// //                         player.jumpsLeft--;
// //                         player.onGround = false;
// //                     }
// //                     if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT && !player.isDashing && player.dashCooldown <= 0)
// //                     {
// //                         player.isDashing = true;
// //                         player.dashTimer = DASH_DURATION;
// //                         player.dashCooldown = DASH_COOLDOWN;
// //                         player.vx = player.facingRight ? DASH_SPEED : -DASH_SPEED;
// //                         player.vy = 0;
// //                     }
// //                     if (e.key.keysym.scancode == SDL_SCANCODE_Z && player.attackTimer <= 0)
// //                     {
// //                         player.attackTimer = MELEE_COOLDOWN;
// //                         player.meleeVisualTimer = 0.1f;
// //                     }
// //                     if (e.key.keysym.scancode == SDL_SCANCODE_X && player.shootTimer <= 0)
// //                     {
// //                         player.shootTimer = SHOOT_COOLDOWN;
// //                         Projectile p;
// //                         p.x = player.x + player.w / 2;
// //                         p.y = player.y + player.h / 2;
// //                         p.vx = player.facingRight ? PROJECTILE_SPEED : -PROJECTILE_SPEED;
// //                         p.vy = 0;
// //                         p.active = true;
// //                         p.fromPlayer = true;
// //                         projectiles.push_back(p);
// //                     }
// //                 }
// //             }
// //         }

// //         if (currentState == GameState::Playing)
// //         {
// //             updatePhysics(dt);
// //             if (fadingOut)
// //             {
// //                 fadeAlpha += FADE_SPEED * dt;
// //                 if (fadeAlpha >= 255)
// //                 {
// //                     resetLevel(nextLevelTarget);
// //                     fadingOut = false;
// //                 }
// //             }
// //             else if (fadeAlpha > 0)
// //                 fadeAlpha -= FADE_SPEED * dt;

// //             if (cameraShakeTimer > 0)
// //             {
// //                 cameraShakeTimer -= dt;
// //                 cameraX += (rand() % 10 - 5);
// //                 cameraY += (rand() % 10 - 5);
// //             }
// //         }

// //         float targetCamX = player.x - WINDOW_WIDTH / 2;
// //         float targetCamY = player.y - WINDOW_HEIGHT / 2;
// //         cameraX += (targetCamX - cameraX) * 0.1f;
// //         cameraY += (targetCamY - cameraY) * 0.1f;

// //         SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
// //         SDL_RenderClear(renderer);

// //         if (currentState == GameState::Menu)
// //         {
// //             int y = 50;
// //             renderTextCentered("Reflex Rift", y, {255, 255, 255, 255}, 1.5f);
// //             y += 50;
// //             renderTextCentered("--- CONTROLS ---", y, {180, 180, 180, 255}, 0.8f);
// //             y += 25;
// //             renderTextCentered("Arrows: Move | Space: Jump | Shift: Dash", y, {180, 180, 180, 255}, 0.8f);
// //             y += 25;
// //             renderTextCentered("Z: Melee | X: Fireball", y, {180, 180, 180, 255}, 0.8f);
// //             y += 25;
// //             renderTextCentered("L: Toggle Lights", y, {180, 180, 180, 255}, 0.8f);
// //             y += 40;

// //             if (saveData.gameCompleted)
// //             {
// //                 renderTextCentered("GAME COMPLETED! SELECT LEVEL:", y, {255, 215, 0, 255});
// //                 y += 40;
// //                 for (int i = 0; i < levels.size(); i++)
// //                 {
// //                     std::string lvlName = "Level " + std::to_string(i + 1) + ": " + levels[i].name;
// //                     SDL_Color c = (i == menuSelection) ? SDL_Color{100, 255, 100, 255} : SDL_Color{180, 180, 180, 255};
// //                     if (i == menuSelection)
// //                         lvlName = "> " + lvlName + " <";
// //                     renderTextCentered(lvlName, y, c);
// //                     y += 30;
// //                 }
// //             }
// //             else
// //             {
// //                 renderTextCentered("Press ENTER to Start", y, {255, 215, 0, 255});
// //             }
// //         }
// //         else if (currentState == GameState::Playing)
// //         {
// //             // Sky Background
// //             SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // SMB Sky Blue
// //             SDL_RenderClear(renderer);

// //             Level &level = levels[currentLevelIndex];
// //             auto &tiles = (currentContext == MapContext::Main) ? level.tiles : activeHidden->tiles;
// //             auto &enemies = (currentContext == MapContext::Main) ? level.enemies : activeHidden->enemies;

// //             // Draw Tiles
// //             for (auto &t : tiles)
// //             {
// //                 if (!t.active)
// //                     continue;
// //                 SDL_Rect d = getRect(t.x - cameraX, t.y - cameraY, TILE_WORLD, TILE_WORLD);
// //                 SDL_Rect s = getSrcRect(t.type);
// //                 SDL_RenderCopy(renderer, tilesTex, &s, &d);
// //             }

// //             // --- DRAW LIGHT GLOWS (NEW) ---
// //             for (auto &t : tiles)
// //             {
// //                 if (!t.active)
// //                     continue;
// //                 /* --- KEYS GLOW COMMENTED OUT ---
// //                 if (t.type == TileType::Key)
// //                 {
// //                     SDL_Rect d = getRect(t.x - cameraX - 16, t.y - cameraY - 16, 64, 64);
// //                     SDL_SetTextureColorMod(glowTex, 255, 215, 0); // GOLD
// //                     SDL_RenderCopy(renderer, glowTex, nullptr, &d);
// //                 }
// //                 */
// //                 if (t.type == TileType::LevelEnd)
// //                 {
// //                     SDL_Rect d = getRect(t.x - cameraX - 16, t.y - cameraY - 16, 64, 64);
// //                     SDL_SetTextureColorMod(glowTex, 0, 255, 0); // GREEN
// //                     SDL_RenderCopy(renderer, glowTex, nullptr, &d);
// //                 }
// //             }

// //             for (auto &e : enemies)
// //             {
// //                 if (!e.active)
// //                     continue;
// //                 SDL_Rect d = getRect(e.x - cameraX, e.y - cameraY, e.w, e.h);
// //                 SDL_Rect s = getSrcRect(e.type);
// //                 SDL_RenderCopyEx(renderer, tilesTex, &s, &d, 0, nullptr, (e.vx > 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
// //             }
// //             if (level.boss.active)
// //             {
// //                 SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
// //                 SDL_Rect bRect = getRect(level.boss.x - cameraX, level.boss.y - cameraY, level.boss.w, level.boss.h);
// //                 SDL_RenderFillRect(renderer, &bRect);
// //             }

// //             SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
// //             for (auto &p : projectiles)
// //             {
// //                 if (!p.active)
// //                     continue;
// //                 SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 8, 8);
// //                 SDL_RenderFillRect(renderer, &r);
// //             }
// //             for (auto &p : particles)
// //             {
// //                 SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, (Uint8)(p.life * 255));
// //                 SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 4, 4);
// //                 SDL_RenderFillRect(renderer, &r);
// //             }

// //             // Player
// //             int pRow = (!player.onGround) ? 1 : 0;
// //             if (SDL_GetTicks() - player.lastAnimTick > ANIM_SPEED_PLAYER)
// //             {
// //                 player.animFrame = (player.animFrame + 1) % 2;
// //                 player.lastAnimTick = SDL_GetTicks();
// //             }
// //             SDL_Rect pSrc = {player.animFrame * 16, pRow * 16, 16, 16};
// //             if (player.isCrouching)
// //                 pSrc.h = 16;
// //             SDL_Rect pDst = getRect(player.x - cameraX, player.y - cameraY, player.w, player.h);
// //             SDL_RenderCopyEx(renderer, playerTex, &pSrc, &pDst, 0, nullptr, player.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

// //             if (player.meleeVisualTimer > 0)
// //             {
// //                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
// //                 SDL_Rect swing = {(int)(pDst.x + (player.facingRight ? pDst.w : -MELEE_RANGE)), pDst.y, (int)MELEE_RANGE, pDst.h};
// //                 SDL_RenderFillRect(renderer, &swing);
// //             }

// //             // Dark Mode Lighting
// //             if (enableLighting && level.isDark)
// //             {
// //                 SDL_SetRenderTarget(renderer, lightLayer);
// //                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
// //                 SDL_RenderClear(renderer);
// //                 SDL_Rect lDst = {(int)(player.x - cameraX + player.w / 2 - 150), (int)(player.y - cameraY + player.h / 2 - 150), 300, 300};
// //                 SDL_RenderCopy(renderer, lightMask, nullptr, &lDst);
// //                 SDL_SetRenderTarget(renderer, nullptr);
// //                 SDL_RenderCopy(renderer, lightLayer, nullptr, nullptr);
// //             }

// //             // HUD
// //             if (font)
// //             {
// //                 // UPDATED HUD: Removed Key count
// //                 std::string hud = "HP: " + std::to_string(player.hp);
// //                 SDL_Surface *s = TTF_RenderText_Blended(font, hud.c_str(), {255, 255, 255, 255});
// //                 SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
// //                 SDL_Rect d = {20, 20, s->w, s->h};
// //                 SDL_RenderCopy(renderer, t, nullptr, &d);
// //                 SDL_FreeSurface(s);
// //                 SDL_DestroyTexture(t);
// //             }
// //             if (fadeAlpha > 0)
// //             {
// //                 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
// //                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)fadeAlpha);
// //                 SDL_Rect fs = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
// //                 SDL_RenderFillRect(renderer, &fs);
// //             }
// //         }
// //         else if (currentState == GameState::Win)
// //         {
// //             renderTextCentered("ALL LEVELS CLEARED!", WINDOW_HEIGHT / 2 - 30, {255, 215, 0, 255}, 2.0f);
// //             renderTextCentered("Press ENTER to return to Menu", WINDOW_HEIGHT / 2 + 40, {200, 200, 200, 255}, 1.0f);
// //         }

// //         SDL_RenderPresent(renderer);
// //     }

// //     SDL_DestroyTexture(glowTex);
// //     SDL_DestroyTexture(lightLayer);
// //     SDL_DestroyTexture(lightMask);
// //     SDL_DestroyTexture(tilesTex);
// //     SDL_DestroyTexture(playerTex);
// //     TTF_CloseFont(font);
// //     SDL_DestroyRenderer(renderer);
// //     SDL_DestroyWindow(window);
// //     IMG_Quit();
// //     TTF_Quit();
// //     SDL_Quit();
// //     return 0;
// // }

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>
// #include <nlohmann/json.hpp>

// #include <vector>
// #include <fstream>
// #include <iostream>
// #include <cmath>
// #include <random>
// #include <algorithm>

// using json = nlohmann::json;

// /* ---------------------------------------------------
//    CONSTANTS & CONFIG
// --------------------------------------------------- */
// constexpr int WINDOW_WIDTH = 1280;
// constexpr int WINDOW_HEIGHT = 720;

// constexpr int TILE_SIZE = 16;
// constexpr int TILE_SCALE = 2;
// constexpr int TILE_WORLD = TILE_SIZE * TILE_SCALE;

// constexpr int TILESET_TILE_SIZE = 16;
// constexpr int TILESET_COLUMNS = 9;

// // Physics
// constexpr float GRAVITY = 0.981f;
// constexpr float MOVE_SPEED = 6.0f;
// constexpr float JUMP_FORCE = -21.0f;
// constexpr float DASH_SPEED = 15.0f;
// constexpr float DASH_DURATION = 0.2f;
// constexpr float DASH_COOLDOWN = 1.0f;

// // Combat
// constexpr float MELEE_RANGE = 40.0f;
// constexpr float MELEE_COOLDOWN = 0.4f;
// constexpr float SHOOT_COOLDOWN = 0.6f;
// constexpr float PROJECTILE_SPEED = 10.0f;

// // Entity Config
// constexpr float ENEMY_SPEED = 2.0f;
// constexpr float FLY_SPEED = 3.0f;
// constexpr float TURRET_RATE = 2.0f;

// // Boss Config
// constexpr int BOSS_HP_MAX = 50;
// constexpr float BOSS_SPEED = 3.5f;

// // Visuals
// constexpr int ANIM_SPEED_PLAYER = 120;
// constexpr int ANIM_SPEED_FIRE = 150;
// constexpr float SHAKE_DURATION = 0.5f;
// constexpr float FADE_SPEED = 1500.0f;

// /* ---------------------------------------------------
//    ENUMS
// --------------------------------------------------- */
// enum class GameState
// {
//     Menu,
//     Playing,
//     Transition,
//     GameOver,
//     Win
// };

// enum class TileType
// {
//     Ground,
//     Spike,
//     Checkpoint,
//     Fire,
//     Key,
//     Breakable,
//     LevelEnd,
//     Spring,
//     EnemyPatrol,
//     EnemyFly,
//     Turret,
//     MovingPlatform,
//     FallingPlatform,
//     EnemyShield,
//     Boss
// };

// enum class MapContext
// {
//     Main,
//     Hidden
// };
// enum class PlayerAnim
// {
//     Idle,
//     Run,
//     Jump,
//     Crouch,
//     Dash,
//     Attack
// };
// enum class BossState
// {
//     Idle,
//     Charge,
//     Shoot
// };

// /* ---------------------------------------------------
//    STRUCTS
// --------------------------------------------------- */
// struct Tile
// {
//     float x{}, y{};
//     TileType type{};
//     bool active{true};
//     float minX{}, maxX{}, speed{};
//     int dir{1};
//     float fallTimer{0.0f};
//     bool shaking{false}, falling{false};
//     float originalX{}, originalY{};
// };

// struct Particle
// {
//     float x, y, vx, vy, life;
//     SDL_Color color;
//     bool isDebris = false; // New: To apply gravity to block chunks
// };

// struct Projectile
// {
//     float x, y, vx, vy;
//     bool active;
//     bool fromPlayer;
// };

// struct Enemy
// {
//     float x, y, vx, vy;
//     float startX;
//     int w = TILE_WORLD, h = TILE_WORLD;
//     TileType type;
//     bool active = true;
//     float timer = 0.0f;
//     int hp = 1;
//     bool facingRight = false;
// };

// struct Boss
// {
//     float x, y, vx, vy;
//     int w = 64, h = 64;
//     int hp = BOSS_HP_MAX;
//     BossState state = BossState::Idle;
//     float stateTimer = 0.0f;
//     bool active = false;
// };

// struct Player
// {
//     float x{}, y{};
//     float vx{}, vy{};
//     int w{TILE_WORLD}, h{TILE_WORLD};
//     int hp = 3;

//     bool onGround{false};
//     int jumpsLeft{2};
//     bool facingRight{true};

//     bool isCrouching{false};
//     bool isDashing{false};
//     float dashTimer{0.0f};
//     float dashCooldown{0.0f};

//     float attackTimer{0.0f};
//     float meleeVisualTimer{0.0f};
//     float shootTimer{0.0f};

//     PlayerAnim anim{PlayerAnim::Idle};
//     int animFrame{0};
//     Uint32 lastAnimTick{0};
// };

// struct HiddenArea
// {
//     std::vector<Tile> tiles;
//     std::vector<Enemy> enemies;
//     SDL_Rect bounds{};
// };

// struct Level
// {
//     std::string name;
//     std::vector<Tile> tiles;
//     std::vector<Enemy> enemies;
//     std::vector<HiddenArea> hiddenAreas;
//     Boss boss;
//     // int totalKeys = 1; // COMMENTED OUT: Keys logic disabled
//     bool isDark{false};
// };

// struct SaveData
// {
//     int totalDeaths = 0;
//     int highestLevel = 0;
//     bool gameCompleted = false;
// };

// /* ---------------------------------------------------
//    GLOBALS
// --------------------------------------------------- */
// GameState currentState = GameState::Menu;
// std::vector<Level> levels;
// int currentLevelIndex = 0;
// MapContext currentContext = MapContext::Main;
// HiddenArea *activeHidden = nullptr;

// Player player;
// std::vector<Particle> particles;
// std::vector<Projectile> projectiles;
// SaveData saveData;

// float cameraX = 0, cameraY = 0;
// float fadeAlpha = 0.0f;
// bool fadingOut = false;
// int nextLevelTarget = -1;
// float checkpointX = 0, checkpointY = 0;
// // int keysCollected = 0; // COMMENTED OUT: Keys logic disabled
// float cameraShakeTimer = 0.0f;

// // Settings
// bool enableLighting = true;
// int menuSelection = 0;

// /* ---------------------------------------------------
//    UTILS
// --------------------------------------------------- */
// SDL_Rect getRect(float x, float y, int w, int h)
// {
//     return {(int)std::round(x), (int)std::round(y), w, h};
// }

// bool checkCollision(SDL_Rect a, SDL_Rect b)
// {
//     return SDL_HasIntersection(&a, &b);
// }

// // Spawns standard sparks/blood/dust
// void spawnParticles(float x, float y, int count, SDL_Color color)
// {
//     for (int i = 0; i < count; i++)
//     {
//         Particle p;
//         p.x = x;
//         p.y = y;
//         p.vx = (rand() % 10 - 5) * 0.5f;
//         p.vy = (rand() % 10 - 5) * 0.5f - 1.0f;
//         p.life = 1.0f;
//         p.color = color;
//         p.isDebris = false;
//         particles.push_back(p);
//     }
// }

// // Spawns breaking block chunks (Gravity affected)
// void spawnDebris(float x, float y)
// {
//     // Create 4 pieces (Top-Left, Top-Right, Bottom-Left, Bottom-Right)
//     for (int i = 0; i < 4; i++)
//     {
//         Particle p;
//         p.x = x + TILE_WORLD / 2;
//         p.y = y + TILE_WORLD / 2;
//         // Spread outwards
//         p.vx = (i % 2 == 0 ? -1 : 1) * (rand() % 3 + 2);
//         // Upward pop
//         p.vy = (i < 2 ? -8.0f : -4.0f);
//         p.life = 2.0f;               // Last longer
//         p.color = {160, 80, 0, 255}; // Brick Brown color
//         p.isDebris = true;
//         particles.push_back(p);
//     }
// }

// TileType parseType(const std::string &name)
// {
//     if (name == "Spring")
//         return TileType::Spring;
//     if (name == "EnemyPatrol")
//         return TileType::EnemyPatrol;
//     if (name == "EnemyFly")
//         return TileType::EnemyFly;
//     if (name == "EnemyShield")
//         return TileType::EnemyShield;
//     if (name == "Turret")
//         return TileType::Turret;
//     if (name == "Boss")
//         return TileType::Boss;
//     if (name == "Spikes")
//         return TileType::Spike;
//     if (name == "Checkpoints")
//         return TileType::Checkpoint;
//     if (name == "Fire")
//         return TileType::Fire;
//     if (name == "Keys")
//         return TileType::Key;
//     if (name == "Breakable")
//         return TileType::Breakable;
//     if (name == "LevelEnd")
//         return TileType::LevelEnd;
//     if (name == "MovingPlatforms")
//         return TileType::MovingPlatform;
//     if (name == "FallingPlatform")
//         return TileType::FallingPlatform;
//     return TileType::Ground;
// }

// SDL_Rect getSrcRect(TileType type)
// {
//     int idx = 1;
//     switch (type)
//     {
//     case TileType::Spring:
//         idx = 8;
//         break;
//     case TileType::Spike:
//         idx = 68;
//         break;
//     case TileType::Checkpoint:
//         idx = 50;
//         break;
//     case TileType::Fire:
//         idx = 54 + (SDL_GetTicks() / ANIM_SPEED_FIRE % 2);
//         break;
//     case TileType::Key:
//         idx = 86;
//         break;
//     case TileType::Breakable:
//         idx = 21;
//         break;
//     case TileType::LevelEnd:
//         idx = 52;
//         break;
//     case TileType::MovingPlatform:
//         idx = 13;
//         break;
//     case TileType::FallingPlatform:
//         idx = 14;
//         break;
//     case TileType::EnemyPatrol:
//         idx = 67;
//         break;
//     case TileType::EnemyFly:
//         idx = 79;
//         break;
//     case TileType::EnemyShield:
//         idx = 82;
//         break;
//     case TileType::Turret:
//         idx = 80;
//         break;
//     case TileType::Boss:
//         idx = 88;
//         break;
//     default:
//         idx = 1;
//         break;
//     }
//     return {(idx % TILESET_COLUMNS) * TILESET_TILE_SIZE, (idx / TILESET_COLUMNS) * TILESET_TILE_SIZE, TILESET_TILE_SIZE, TILESET_TILE_SIZE};
// }

// void saveGame()
// {
//     json j;
//     j["totalDeaths"] = saveData.totalDeaths;
//     j["highestLevel"] = saveData.highestLevel;
//     j["gameCompleted"] = saveData.gameCompleted;
//     std::ofstream o("savegame.json");
//     o << j;
// }

// void loadSave()
// {
//     std::ifstream i("savegame.json");
//     if (i.is_open())
//     {
//         json j;
//         i >> j;
//         saveData.totalDeaths = j.value("totalDeaths", 0);
//         saveData.highestLevel = j.value("highestLevel", 0);
//         saveData.gameCompleted = j.value("gameCompleted", false);
//     }
// }

// std::vector<Level> parseMap(const std::string &path)
// {
//     std::vector<Level> lvls;
//     std::ifstream f(path);
//     if (!f.is_open())
//         return lvls;
//     json j;
//     f >> j;

//     for (auto &lj : j["levels"])
//     {
//         Level lvl;
//         lvl.name = lj["name"];
//         // lvl.totalKeys = 1; // COMMENTED OUT: Keys logic disabled
//         if (lj.contains("isDark"))
//             lvl.isDark = lj["isDark"];

//         for (auto &layer : lj["layers"])
//         {
//             bool isHidden = (layer["name"] == "HiddenArea");
//             HiddenArea ha;
//             if (isHidden)
//             {
//                 auto b = layer["bounds"];
//                 ha.bounds = {b["x"].get<int>() * TILE_WORLD, b["y"].get<int>() * TILE_WORLD, b["w"].get<int>() * TILE_WORLD, b["h"].get<int>() * TILE_WORLD};
//             }

//             for (auto &t : layer["tiles"])
//             {
//                 float x = t["x"].get<float>() * TILE_WORLD;
//                 float y = t["y"].get<float>() * TILE_WORLD;
//                 std::string typeStr = t.contains("type") ? t["type"] : (layer["name"] == "Entities" ? "Entity" : "Surface");
//                 if (!t.contains("type") && layer["name"] != "Surface" && layer["name"] != "Entities" && !isHidden)
//                     typeStr = layer["name"];

//                 TileType type = parseType(typeStr);

//                 if (type == TileType::Boss)
//                 {
//                     lvl.boss.active = true;
//                     lvl.boss.x = x;
//                     lvl.boss.y = y;
//                     lvl.boss.hp = BOSS_HP_MAX;
//                 }
//                 else if (type == TileType::EnemyPatrol || type == TileType::EnemyFly || type == TileType::EnemyShield || type == TileType::Turret)
//                 {
//                     Enemy e{x, y, 0, 0, x, TILE_WORLD, TILE_WORLD, type};
//                     if (isHidden)
//                         ha.enemies.push_back(e);
//                     else
//                         lvl.enemies.push_back(e);
//                 }
//                 else
//                 {
//                     Tile tile{x, y, type};
//                     tile.originalX = x;
//                     tile.originalY = y;
//                     if (type == TileType::MovingPlatform)
//                     {
//                         tile.minX = t["minX"];
//                         tile.maxX = t["maxX"];
//                         tile.speed = t["speed"];
//                     }
//                     if (isHidden)
//                         ha.tiles.push_back(tile);
//                     else
//                         lvl.tiles.push_back(tile);
//                 }
//             }
//             if (isHidden)
//                 lvl.hiddenAreas.push_back(ha);
//         }
//         lvls.push_back(lvl);
//     }
//     return lvls;
// }

// /* ---------------------------------------------------
//    LOGIC
// --------------------------------------------------- */
// void resetLevel(int idx)
// {
//     // Safety check against empty levels vector
//     if (levels.empty())
//         return;

//     if (idx >= levels.size())
//     {
//         saveData.gameCompleted = true;
//         saveGame();
//         currentState = GameState::Win;
//         return;
//     }

//     currentLevelIndex = idx;
//     currentContext = MapContext::Main;
//     activeHidden = nullptr;
//     // keysCollected = 0; // COMMENTED OUT: Keys logic disabled
//     projectiles.clear();
//     particles.clear();

//     Level &lvl = levels[idx];
//     for (auto &e : lvl.enemies)
//     {
//         e.active = true;
//         e.x = e.startX;
//         e.hp = 1;
//     }
//     lvl.boss.active = (lvl.boss.hp > 0);
//     if (lvl.boss.active)
//     {
//         lvl.boss.hp = BOSS_HP_MAX;
//         lvl.boss.state = BossState::Idle;
//     }

//     for (auto &t : lvl.tiles)
//     {
//         if (t.type == TileType::Key)
//             t.active = true;
//         if (t.type == TileType::FallingPlatform)
//         {
//             t.active = true;
//             t.y = t.originalY;
//             t.falling = false;
//             t.shaking = false;
//         }
//         // Ensure breakables are reset
//         if (t.type == TileType::Breakable)
//         {
//             t.active = true;
//         }
//     }

//     // Default Spawn
//     player.x = 2 * TILE_WORLD;
//     player.y = 10 * TILE_WORLD;

//     // Level 2 special spawn (Drop in)
//     if (idx == 1)
//     {
//         player.x = 3 * TILE_WORLD;
//         player.y = 2 * TILE_WORLD;
//     }

//     player.vx = 0;
//     player.vy = 0;
//     player.hp = 3;
//     checkpointX = player.x;
//     checkpointY = player.y;
//     cameraX = player.x;
//     cameraY = player.y;

//     if (idx > saveData.highestLevel)
//         saveData.highestLevel = idx;
//     saveGame();
// }

// void triggerTransition(int nextIdx)
// {
//     fadingOut = true;
//     nextLevelTarget = nextIdx;
// }

// void respawn()
// {
//     saveData.totalDeaths++;
//     player.x = checkpointX;
//     player.y = checkpointY;
//     player.vx = 0;
//     player.vy = 0;
//     player.hp = 3;
//     currentContext = MapContext::Main;
//     activeHidden = nullptr;
//     spawnParticles(player.x, player.y, 15, {255, 50, 50, 255});
//     fadeAlpha = 255.0f;
//     fadingOut = false;
//     cameraShakeTimer = 0.5f;
// }

// void damageEnemy(Enemy &e)
// {
//     e.hp--;
//     if (e.hp <= 0)
//     {
//         e.active = false;
//         spawnParticles(e.x, e.y, 10, {255, 100, 0, 255});
//     }
//     else
//         spawnParticles(e.x, e.y, 5, {200, 200, 200, 255});
// }

// void damagePlayer()
// {
//     if (player.dashTimer > 0)
//         return;
//     player.hp--;
//     cameraShakeTimer = 0.3f;
//     spawnParticles(player.x, player.y, 10, {255, 0, 0, 255});
//     if (player.hp <= 0)
//         respawn();
// }

// void updatePhysics(float dt)
// {
//     const Uint8 *k = SDL_GetKeyboardState(nullptr);

//     if (player.dashCooldown > 0)
//         player.dashCooldown -= dt;
//     if (player.attackTimer > 0)
//         player.attackTimer -= dt;
//     if (player.meleeVisualTimer > 0)
//         player.meleeVisualTimer -= dt;
//     if (player.shootTimer > 0)
//         player.shootTimer -= dt;

//     if (player.isDashing)
//     {
//         player.dashTimer -= dt;
//         if (player.dashTimer <= 0)
//         {
//             player.isDashing = false;
//             player.vx = 0;
//         }
//         else
//         {
//             spawnParticles(player.x + player.w / 2, player.y + player.h / 2, 1, {100, 200, 255, 100});
//             return;
//         }
//     }

//     float speed = MOVE_SPEED;
//     player.isCrouching = k[SDL_SCANCODE_S] || k[SDL_SCANCODE_DOWN];
//     player.h = player.isCrouching ? TILE_WORLD / 2 : TILE_WORLD;
//     if (player.isCrouching)
//         speed *= 0.5f;

//     player.vx = 0;

//     if (k[SDL_SCANCODE_LEFT])
//     {
//         player.vx = -speed;
//         player.facingRight = false;
//     }
//     if (k[SDL_SCANCODE_RIGHT])
//     {
//         player.vx = speed;
//         player.facingRight = true;
//     }
//     player.vy += GRAVITY;

//     Level &lvl = levels[currentLevelIndex];
//     auto &tiles = (currentContext == MapContext::Main) ? lvl.tiles : activeHidden->tiles;

//     for (auto &t : tiles)
//     {
//         if (!t.active)
//             continue;
//         if (t.type == TileType::MovingPlatform)
//         {
//             t.x += t.speed * t.dir;
//             if (t.x >= t.maxX)
//                 t.dir = -1;
//             if (t.x <= t.minX)
//                 t.dir = 1;
//         }
//         if (t.type == TileType::FallingPlatform && t.shaking)
//         {
//             t.fallTimer += dt;
//             t.x = t.originalX + (rand() % 3 - 1);
//             if (t.fallTimer > 0.5f)
//             {
//                 t.falling = true;
//                 t.shaking = false;
//             }
//         }
//         if (t.type == TileType::FallingPlatform && t.falling)
//         {
//             t.y += 5.0f;
//             if (t.y > lvl.tiles.back().y + 500)
//                 t.active = false;
//         }
//     }

//     player.x += player.vx;
//     SDL_Rect pRect = getRect(player.x, player.y, player.w, player.h);
//     for (auto &t : tiles)
//     {
//         if (!t.active || t.type == TileType::Key || t.type == TileType::Checkpoint || t.type == TileType::Spike || t.type == TileType::Fire || t.type == TileType::Spring || t.type == TileType::LevelEnd)
//             continue;
//         if (t.type == TileType::FallingPlatform && t.falling)
//             continue;
//         SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);
//         if (checkCollision(pRect, tRect))
//         {
//             if (player.vx > 0)
//                 player.x = tRect.x - player.w;
//             if (player.vx < 0)
//                 player.x = tRect.x + tRect.w;
//             player.vx = 0;
//         }
//     }

//     player.y += player.vy;
//     player.onGround = false;
//     pRect = getRect(player.x, player.y, player.w, player.h);
//     for (auto &t : tiles)
//     {
//         if (!t.active)
//             continue;
//         SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);

//         /* --- KEYS COMMENTED OUT ---
//         if (t.type == TileType::Key)
//         {
//             if (checkCollision(pRect, tRect))
//             {
//                 t.active = false;
//                 keysCollected++;
//                 spawnParticles(t.x, t.y, 10, {255, 215, 0, 255});
//             }
//             continue;
//         }
//         */

//         if (t.type == TileType::Spring)
//         {
//             SDL_Rect sRect = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
//             if (checkCollision(pRect, sRect) && player.vy > 0)
//             {
//                 player.vy = -30.0f;
//                 player.jumpsLeft = 2;
//                 spawnParticles(player.x, player.y + player.h, 8, {50, 255, 50, 255});
//             }
//             continue;
//         }
//         if (t.type == TileType::Spike || t.type == TileType::Fire)
//         {
//             SDL_Rect hurtBox = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
//             if (checkCollision(pRect, hurtBox))
//                 damagePlayer();
//             continue;
//         }
//         if (t.type == TileType::LevelEnd)
//         {
//             if (checkCollision(pRect, tRect))
//             {
//                 if (currentContext == MapContext::Hidden)
//                 {
//                     currentContext = MapContext::Main;
//                     activeHidden = nullptr;
//                     player.x = checkpointX;
//                     player.y = checkpointY;
//                 }
//                 // CHANGED: Removed key check. Simple door logic.
//                 // else if (keysCollected >= 1)
//                 else
//                 {
//                     triggerTransition(currentLevelIndex + 1);
//                 }
//             }
//             continue;
//         }
//         if (t.type == TileType::Checkpoint)
//         {
//             if (checkCollision(pRect, tRect))
//             {
//                 checkpointX = t.x;
//                 checkpointY = t.y;
//             }
//             continue;
//         }

//         if (t.type == TileType::MovingPlatform)
//         {
//             if (player.x + player.w > t.x && player.x < t.x + TILE_WORLD && player.y + player.h >= t.y - 4 && player.y + player.h <= t.y + 10 && player.vy >= 0)
//             {
//                 player.y = t.y - player.h;
//                 player.vy = 0;
//                 player.onGround = true;
//                 player.jumpsLeft = 2;
//                 player.x += t.speed * t.dir;
//                 continue;
//             }
//         }
//         if (t.type == TileType::FallingPlatform && t.falling)
//             continue;

//         if (checkCollision(pRect, tRect))
//         {
//             if (t.type == TileType::FallingPlatform && player.vy > 0)
//                 t.shaking = true;
//             if (player.vy > 0)
//             {
//                 player.y = tRect.y - player.h;
//                 player.vy = 0;
//                 player.onGround = true;
//                 player.jumpsLeft = 2;
//             }
//             else if (player.vy < 0)
//             {
//                 // Ceiling collision (Head hit)
//                 player.y = tRect.y + tRect.h;
//                 player.vy = 0;

//                 // --- BREAKABLE TILE LOGIC ---
//                 if (t.type == TileType::Breakable)
//                 {
//                     t.active = false;      // Destroy the tile
//                     spawnDebris(t.x, t.y); // Spawn animation debris

//                     // If this breakable block hides a secret area
//                     if (!lvl.hiddenAreas.empty())
//                     {
//                         currentContext = MapContext::Hidden;
//                         activeHidden = &lvl.hiddenAreas[0];
//                         player.x = activeHidden->bounds.x + 64;
//                         player.y = activeHidden->bounds.y + 64;
//                     }
//                 }
//             }
//         }
//     }

//     if (player.y > 2000)
//         respawn();

//     auto &enemies = (currentContext == MapContext::Main) ? lvl.enemies : activeHidden->enemies;
//     pRect = getRect(player.x, player.y, player.w, player.h);

//     if (player.meleeVisualTimer > 0)
//     {
//         SDL_Rect attackBox = {(int)(player.x + (player.facingRight ? player.w : -MELEE_RANGE)), (int)player.y, (int)MELEE_RANGE, player.h};
//         for (auto &e : enemies)
//         {
//             if (!e.active)
//                 continue;
//             if (checkCollision(attackBox, getRect(e.x, e.y, e.w, e.h)))
//             {
//                 bool blocked = false;
//                 if (e.type == TileType::EnemyShield)
//                 {
//                     bool hitFromLeft = player.x < e.x;
//                     bool enemyFacingLeft = e.vx < 0;
//                     if (hitFromLeft == enemyFacingLeft)
//                         blocked = true;
//                 }
//                 if (blocked)
//                     spawnParticles(e.x, e.y, 2, {100, 100, 255, 255});
//                 else
//                     damageEnemy(e);
//             }
//         }
//         if (lvl.boss.active && checkCollision(attackBox, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
//         {
//             lvl.boss.hp--;
//             spawnParticles(lvl.boss.x + lvl.boss.w / 2, lvl.boss.y + lvl.boss.h / 2, 5, {255, 0, 0, 255});
//             if (lvl.boss.hp <= 0)
//             {
//                 lvl.boss.active = false;
//                 triggerTransition(currentLevelIndex + 1);
//             }
//         }
//     }

//     for (auto &e : enemies)
//     {
//         if (!e.active)
//             continue;
//         SDL_Rect eRect = getRect(e.x, e.y, e.w, e.h);

//         if (e.type == TileType::EnemyPatrol || e.type == TileType::EnemyShield)
//         {
//             if (e.vx == 0)
//                 e.vx = ENEMY_SPEED;
//             e.x += e.vx;
//             if (e.x > e.startX + 64)
//                 e.vx = -ENEMY_SPEED;
//             if (e.x < e.startX - 64)
//                 e.vx = ENEMY_SPEED;
//             if (checkCollision(pRect, eRect))
//                 damagePlayer();
//         }
//         else if (e.type == TileType::EnemyFly)
//         {
//             float dist = std::sqrt(std::pow(player.x - e.x, 2) + std::pow(player.y - e.y, 2));
//             if (dist < 300.0f)
//             {
//                 float angle = std::atan2(player.y - e.y, player.x - e.x);
//                 e.x += std::cos(angle) * FLY_SPEED * 0.5f;
//                 e.y += std::sin(angle) * FLY_SPEED * 0.5f;
//             }
//             if (checkCollision(pRect, eRect))
//                 damagePlayer();
//         }
//         else if (e.type == TileType::Turret)
//         {
//             e.timer += dt;
//             if (e.timer > TURRET_RATE)
//             {
//                 e.timer = 0;
//                 Projectile p;
//                 p.x = e.x;
//                 p.y = e.y + 8;
//                 p.vx = (player.x < e.x) ? -4.0f : 4.0f;
//                 p.vy = 0;
//                 p.active = true;
//                 p.fromPlayer = false;
//                 projectiles.push_back(p);
//             }
//         }
//     }

//     if (lvl.boss.active)
//     {
//         Boss &b = lvl.boss;
//         SDL_Rect bRect = getRect(b.x, b.y, b.w, b.h);
//         if (checkCollision(pRect, bRect))
//             damagePlayer();

//         b.stateTimer += dt;
//         switch (b.state)
//         {
//         case BossState::Idle:
//             if (b.stateTimer > 2.0f)
//             {
//                 b.state = BossState::Charge;
//                 b.stateTimer = 0;
//             }
//             break;
//         case BossState::Charge:
//             b.x += (player.x < b.x ? -BOSS_SPEED : BOSS_SPEED);
//             if (b.stateTimer > 2.0f)
//             {
//                 b.state = BossState::Shoot;
//                 b.stateTimer = 0;
//             }
//             break;
//         case BossState::Shoot:
//             if ((int)(b.stateTimer * 10) % 5 == 0)
//             {
//                 Projectile p;
//                 p.x = b.x + b.w / 2;
//                 p.y = b.y + b.h / 2;
//                 p.vx = (rand() % 10 - 5);
//                 p.vy = (rand() % 10 - 5);
//                 p.active = true;
//                 p.fromPlayer = false;
//                 projectiles.push_back(p);
//             }
//             if (b.stateTimer > 2.0f)
//             {
//                 b.state = BossState::Idle;
//                 b.stateTimer = 0;
//             }
//             break;
//         }
//     }

//     for (auto &p : projectiles)
//     {
//         if (!p.active)
//             continue;
//         p.x += p.vx;
//         p.y += p.vy;
//         SDL_Rect r = getRect(p.x, p.y, 8, 8);
//         if (p.fromPlayer)
//         {
//             for (auto &e : enemies)
//             {
//                 if (e.active && checkCollision(r, getRect(e.x, e.y, e.w, e.h)))
//                 {
//                     if (e.type != TileType::EnemyShield)
//                         damageEnemy(e);
//                     p.active = false;
//                 }
//             }
//             if (lvl.boss.active && checkCollision(r, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
//             {
//                 lvl.boss.hp--;
//                 p.active = false;
//             }
//         }
//         else
//         {
//             if (checkCollision(r, pRect))
//             {
//                 damagePlayer();
//                 p.active = false;
//             }
//         }
//         if (std::abs(p.x - player.x) > 600)
//             p.active = false;
//     }

//     for (auto it = particles.begin(); it != particles.end();)
//     {
//         it->x += it->vx;
//         it->y += it->vy;

//         // Apply gravity if it's falling debris
//         if (it->isDebris)
//         {
//             it->vy += 0.5f;
//         }

//         it->life -= 0.05f;
//         if (it->life <= 0)
//             it = particles.erase(it);
//         else
//             ++it;
//     }
// }

// /* ---------------------------------------------------
//    MAIN
// --------------------------------------------------- */
// int main(int argc, char *argv[])
// {
//     SDL_Init(SDL_INIT_VIDEO);
//     IMG_Init(IMG_INIT_PNG);
//     TTF_Init();
//     SDL_Window *window = SDL_CreateWindow("Reflex Rift - Mario Recreations", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
//     SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

//     // CHANGE PATH IF NEEDED
//     std::string path = "/Users/lazycodebaker/Documents/Dev/CPP/platformer_2d_temp/assets/";
//     SDL_Texture *tilesTex = IMG_LoadTexture(renderer, (path + "spritesheet_tiles_resized.png").c_str());
//     SDL_Texture *playerTex = IMG_LoadTexture(renderer, (path + "spritesheet_characters.png").c_str());
//     TTF_Font *font = TTF_OpenFont((path + "PixelifySans-VariableFont_wght.ttf").c_str(), 24);

//     // --- GLOW TEXTURE GENERATION ---
//     // Create a 64x64 texture for the glow lights (Gold/Green)
//     SDL_Texture *glowTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 64);
//     SDL_SetTextureBlendMode(glowTex, SDL_BLENDMODE_ADD);
//     SDL_SetRenderTarget(renderer, glowTex);
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
//     SDL_RenderClear(renderer);
//     // Draw a radial gradient manually
//     for (int y = 0; y < 64; y++)
//     {
//         for (int x = 0; x < 64; x++)
//         {
//             float dist = std::sqrt(std::pow(x - 32, 2) + std::pow(y - 32, 2));
//             if (dist < 32)
//             {
//                 int alpha = (int)(255 * (1.0f - dist / 32.0f));
//                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
//                 SDL_RenderDrawPoint(renderer, x, y);
//             }
//         }
//     }
//     SDL_SetRenderTarget(renderer, nullptr);

//     // Lighting (Shadow Mask)
//     SDL_Texture *lightLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
//     SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
//     SDL_Texture *lightMask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 300);
//     SDL_BlendMode eraser = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
//     SDL_SetTextureBlendMode(lightMask, eraser);
//     SDL_SetRenderTarget(renderer, lightMask);
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
//     SDL_RenderClear(renderer);
//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//     for (int w = 0; w < 300; w++)
//         for (int h = 0; h < 300; h++)
//             if ((150 - w) * (150 - w) + (150 - h) * (150 - h) < 22500)
//                 SDL_RenderDrawPoint(renderer, w, h);

//     SDL_SetRenderTarget(renderer, nullptr);

//     loadSave();
//     // FIXED: Uses correct path now
//     levels = parseMap(path + "map.json");

//     // ADDED SAFETY CHECK for empty map
//     if (levels.empty())
//     {
//         std::cerr << "CRITICAL ERROR: Could not load levels! Check map.json path." << std::endl;
//         Level errorLevel;
//         errorLevel.name = "ERROR: MAP NOT FOUND";
//         errorLevel.boss.active = false;
//         levels.push_back(errorLevel);
//     }

//     auto renderTextCentered = [&](std::string text, int y, SDL_Color color, float scale = 1.0f)
//     {
//         if (!font)
//             return;
//         SDL_Surface *s = TTF_RenderText_Blended(font, text.c_str(), color);
//         if (!s)
//             return;
//         SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
//         int w = (int)(s->w * scale);
//         int h = (int)(s->h * scale);
//         SDL_Rect dst = {WINDOW_WIDTH / 2 - w / 2, y, w, h};
//         SDL_RenderCopy(renderer, t, nullptr, &dst);
//         SDL_FreeSurface(s);
//         SDL_DestroyTexture(t);
//     };

//     bool running = true;
//     Uint32 lastTime = SDL_GetTicks();

//     while (running)
//     {
//         Uint32 currentTime = SDL_GetTicks();
//         float dt = (currentTime - lastTime) / 1000.0f;
//         lastTime = currentTime;

//         SDL_Event e;
//         while (SDL_PollEvent(&e))
//         {
//             if (e.type == SDL_QUIT)
//                 running = false;
//             if (e.type == SDL_KEYDOWN)
//             {
//                 if (currentState == GameState::Menu)
//                 {
//                     if (saveData.gameCompleted)
//                     {
//                         if (e.key.keysym.scancode == SDL_SCANCODE_UP)
//                         {
//                             menuSelection--;
//                             if (menuSelection < 0)
//                                 menuSelection = levels.size() - 1;
//                         }
//                         if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
//                         {
//                             menuSelection++;
//                             if (menuSelection >= levels.size())
//                                 menuSelection = 0;
//                         }
//                         if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
//                         {
//                             currentState = GameState::Playing;
//                             resetLevel(menuSelection);
//                         }
//                     }
//                     else
//                     {
//                         if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
//                         {
//                             currentState = GameState::Playing;
//                             resetLevel((saveData.highestLevel < levels.size()) ? saveData.highestLevel : 0);
//                         }
//                     }
//                     if (e.key.keysym.scancode == SDL_SCANCODE_L)
//                         enableLighting = !enableLighting;
//                 }
//                 else if (currentState == GameState::Win)
//                 {
//                     if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
//                         currentState = GameState::Menu;
//                 }
//                 else if (currentState == GameState::Playing)
//                 {
//                     if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
//                         currentState = GameState::Menu;
//                     if (e.key.keysym.scancode == SDL_SCANCODE_SPACE && player.jumpsLeft > 0)
//                     {
//                         player.vy = JUMP_FORCE;
//                         player.jumpsLeft--;
//                         player.onGround = false;
//                     }
//                     if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT && !player.isDashing && player.dashCooldown <= 0)
//                     {
//                         player.isDashing = true;
//                         player.dashTimer = DASH_DURATION;
//                         player.dashCooldown = DASH_COOLDOWN;
//                         player.vx = player.facingRight ? DASH_SPEED : -DASH_SPEED;
//                         player.vy = 0;
//                     }
//                     if (e.key.keysym.scancode == SDL_SCANCODE_Z && player.attackTimer <= 0)
//                     {
//                         player.attackTimer = MELEE_COOLDOWN;
//                         player.meleeVisualTimer = 0.1f;
//                     }
//                     if (e.key.keysym.scancode == SDL_SCANCODE_X && player.shootTimer <= 0)
//                     {
//                         player.shootTimer = SHOOT_COOLDOWN;
//                         Projectile p;
//                         p.x = player.x + player.w / 2;
//                         p.y = player.y + player.h / 2;
//                         p.vx = player.facingRight ? PROJECTILE_SPEED : -PROJECTILE_SPEED;
//                         p.vy = 0;
//                         p.active = true;
//                         p.fromPlayer = true;
//                         projectiles.push_back(p);
//                     }
//                 }
//             }
//         }

//         if (currentState == GameState::Playing)
//         {
//             updatePhysics(dt);
//             if (fadingOut)
//             {
//                 fadeAlpha += FADE_SPEED * dt;
//                 if (fadeAlpha >= 255)
//                 {
//                     resetLevel(nextLevelTarget);
//                     fadingOut = false;
//                 }
//             }
//             else if (fadeAlpha > 0)
//                 fadeAlpha -= FADE_SPEED * dt;

//             if (cameraShakeTimer > 0)
//             {
//                 cameraShakeTimer -= dt;
//                 cameraX += (rand() % 10 - 5);
//                 cameraY += (rand() % 10 - 5);
//             }
//         }

//         float targetCamX = player.x - WINDOW_WIDTH / 2;
//         float targetCamY = player.y - WINDOW_HEIGHT / 2;
//         cameraX += (targetCamX - cameraX) * 0.1f;
//         cameraY += (targetCamY - cameraY) * 0.1f;

//         SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
//         SDL_RenderClear(renderer);

//         if (currentState == GameState::Menu)
//         {
//             int y = 50;
//             renderTextCentered("Reflex Rift", y, {255, 255, 255, 255}, 1.5f);
//             y += 50;
//             renderTextCentered("--- CONTROLS ---", y, {180, 180, 180, 255}, 0.8f);
//             y += 25;
//             renderTextCentered("Arrows: Move | Space: Jump | Shift: Dash", y, {180, 180, 180, 255}, 0.8f);
//             y += 25;
//             renderTextCentered("Z: Melee | X: Fireball", y, {180, 180, 180, 255}, 0.8f);
//             y += 25;
//             renderTextCentered("L: Toggle Lights", y, {180, 180, 180, 255}, 0.8f);
//             y += 40;

//             if (saveData.gameCompleted)
//             {
//                 renderTextCentered("GAME COMPLETED! SELECT LEVEL:", y, {255, 215, 0, 255});
//                 y += 40;
//                 for (int i = 0; i < levels.size(); i++)
//                 {
//                     std::string lvlName = "Level " + std::to_string(i + 1) + ": " + levels[i].name;
//                     SDL_Color c = (i == menuSelection) ? SDL_Color{100, 255, 100, 255} : SDL_Color{180, 180, 180, 255};
//                     if (i == menuSelection)
//                         lvlName = "> " + lvlName + " <";
//                     renderTextCentered(lvlName, y, c);
//                     y += 30;
//                 }
//             }
//             else
//             {
//                 renderTextCentered("Press ENTER to Start", y, {255, 215, 0, 255});
//             }
//         }
//         else if (currentState == GameState::Playing)
//         {
//             // Sky Background (Reverted to Blue for Mario vibe)
//             SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
//             SDL_RenderClear(renderer);

//             Level &level = levels[currentLevelIndex];
//             auto &tiles = (currentContext == MapContext::Main) ? level.tiles : activeHidden->tiles;
//             auto &enemies = (currentContext == MapContext::Main) ? level.enemies : activeHidden->enemies;

//             // Draw Tiles
//             for (auto &t : tiles)
//             {
//                 if (!t.active)
//                     continue;
//                 SDL_Rect d = getRect(t.x - cameraX, t.y - cameraY, TILE_WORLD, TILE_WORLD);
//                 SDL_Rect s = getSrcRect(t.type);
//                 SDL_RenderCopy(renderer, tilesTex, &s, &d);
//             }

//             // --- DRAW LIGHT GLOWS (NEW) ---
//             for (auto &t : tiles)
//             {
//                 if (!t.active)
//                     continue;
//                 /* --- KEYS GLOW COMMENTED OUT ---
//                 if (t.type == TileType::Key)
//                 {
//                     SDL_Rect d = getRect(t.x - cameraX - 16, t.y - cameraY - 16, 64, 64);
//                     SDL_SetTextureColorMod(glowTex, 255, 215, 0); // GOLD
//                     SDL_RenderCopy(renderer, glowTex, nullptr, &d);
//                 }
//                 */
//                 if (t.type == TileType::LevelEnd)
//                 {
//                     SDL_Rect d = getRect(t.x - cameraX - 16, t.y - cameraY - 16, 64, 64);
//                     SDL_SetTextureColorMod(glowTex, 0, 255, 0); // GREEN
//                     SDL_RenderCopy(renderer, glowTex, nullptr, &d);
//                 }
//             }

//             for (auto &e : enemies)
//             {
//                 if (!e.active)
//                     continue;
//                 SDL_Rect d = getRect(e.x - cameraX, e.y - cameraY, e.w, e.h);
//                 SDL_Rect s = getSrcRect(e.type);
//                 SDL_RenderCopyEx(renderer, tilesTex, &s, &d, 0, nullptr, (e.vx > 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
//             }
//             if (level.boss.active)
//             {
//                 SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
//                 SDL_Rect bRect = getRect(level.boss.x - cameraX, level.boss.y - cameraY, level.boss.w, level.boss.h);
//                 SDL_RenderFillRect(renderer, &bRect);
//             }

//             SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
//             for (auto &p : projectiles)
//             {
//                 if (!p.active)
//                     continue;
//                 SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 8, 8);
//                 SDL_RenderFillRect(renderer, &r);
//             }
//             for (auto &p : particles)
//             {
//                 SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, (Uint8)(p.life * 255));
//                 SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 4, 4);
//                 SDL_RenderFillRect(renderer, &r);
//             }

//             // Player
//             int pRow = (!player.onGround) ? 1 : 0;
//             if (SDL_GetTicks() - player.lastAnimTick > ANIM_SPEED_PLAYER)
//             {
//                 player.animFrame = (player.animFrame + 1) % 2;
//                 player.lastAnimTick = SDL_GetTicks();
//             }
//             SDL_Rect pSrc = {player.animFrame * 16, pRow * 16, 16, 16};
//             if (player.isCrouching)
//                 pSrc.h = 16;
//             SDL_Rect pDst = getRect(player.x - cameraX, player.y - cameraY, player.w, player.h);
//             SDL_RenderCopyEx(renderer, playerTex, &pSrc, &pDst, 0, nullptr, player.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

//             if (player.meleeVisualTimer > 0)
//             {
//                 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
//                 SDL_Rect swing = {(int)(pDst.x + (player.facingRight ? pDst.w : -MELEE_RANGE)), pDst.y, (int)MELEE_RANGE, pDst.h};
//                 SDL_RenderFillRect(renderer, &swing);
//             }

//             // Dark Mode Lighting
//             if (enableLighting && level.isDark)
//             {
//                 SDL_SetRenderTarget(renderer, lightLayer);
//                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
//                 SDL_RenderClear(renderer);
//                 SDL_Rect lDst = {(int)(player.x - cameraX + player.w / 2 - 150), (int)(player.y - cameraY + player.h / 2 - 150), 300, 300};
//                 SDL_RenderCopy(renderer, lightMask, nullptr, &lDst);
//                 SDL_SetRenderTarget(renderer, nullptr);
//                 SDL_RenderCopy(renderer, lightLayer, nullptr, nullptr);
//             }

//             // HUD
//             if (font)
//             {
//                 // UPDATED HUD: Removed Key count
//                 std::string hud = "HP: " + std::to_string(player.hp);
//                 SDL_Surface *s = TTF_RenderText_Blended(font, hud.c_str(), {255, 255, 255, 255});
//                 SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
//                 SDL_Rect d = {20, 20, s->w, s->h};
//                 SDL_RenderCopy(renderer, t, nullptr, &d);
//                 SDL_FreeSurface(s);
//                 SDL_DestroyTexture(t);
//             }
//             if (fadeAlpha > 0)
//             {
//                 SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//                 SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)fadeAlpha);
//                 SDL_Rect fs = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
//                 SDL_RenderFillRect(renderer, &fs);
//             }
//         }
//         else if (currentState == GameState::Win)
//         {
//             renderTextCentered("ALL LEVELS CLEARED!", WINDOW_HEIGHT / 2 - 30, {255, 215, 0, 255}, 2.0f);
//             renderTextCentered("Press ENTER to return to Menu", WINDOW_HEIGHT / 2 + 40, {200, 200, 200, 255}, 1.0f);
//         }

//         SDL_RenderPresent(renderer);
//     }

//     SDL_DestroyTexture(glowTex);
//     SDL_DestroyTexture(lightLayer);
//     SDL_DestroyTexture(lightMask);
//     SDL_DestroyTexture(tilesTex);
//     SDL_DestroyTexture(playerTex);
//     TTF_CloseFont(font);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     IMG_Quit();
//     TTF_Quit();
//     SDL_Quit();
//     return 0;
// }

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

using json = nlohmann::json;

/* ---------------------------------------------------
   CONSTANTS & CONFIG
--------------------------------------------------- */
constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;

constexpr int TILE_SIZE = 16;
constexpr int TILE_SCALE = 2;
constexpr int TILE_WORLD = TILE_SIZE * TILE_SCALE;

constexpr int TILESET_TILE_SIZE = 16;
constexpr int TILESET_COLUMNS = 9;

// Physics
constexpr float GRAVITY = 0.981f;
constexpr float MOVE_SPEED = 6.0f;
constexpr float JUMP_FORCE = -21.0f;
constexpr float DASH_SPEED = 15.0f;
constexpr float DASH_DURATION = 0.2f;
constexpr float DASH_COOLDOWN = 1.0f;

// Combat
constexpr float MELEE_RANGE = 40.0f;
constexpr float MELEE_COOLDOWN = 0.4f;
constexpr float SHOOT_COOLDOWN = 0.6f;
constexpr float PROJECTILE_SPEED = 10.0f;

// Entity Config
constexpr float ENEMY_SPEED = 2.0f;
constexpr float FLY_SPEED = 3.0f;
constexpr float TURRET_RATE = 2.0f;

// Boss Config
constexpr int BOSS_HP_MAX = 50;
constexpr float BOSS_SPEED = 3.5f;

// Visuals
constexpr int ANIM_SPEED_PLAYER = 120;
constexpr int ANIM_SPEED_FIRE = 150;
constexpr float SHAKE_DURATION = 0.5f;
constexpr float FADE_SPEED = 1500.0f;

/* ---------------------------------------------------
   ENUMS
--------------------------------------------------- */
enum class GameState
{
    Menu,
    Playing,
    Transition,
    GameOver,
    Win
};

enum class TileType
{
    Ground,
    Spike,
    Checkpoint,
    Fire,
    Key,
    Breakable,
    LevelEnd,
    Spring,
    EnemyPatrol,
    EnemyFly,
    Turret,
    MovingPlatform,
    FallingPlatform,
    EnemyShield,
    Boss
};

enum class MapContext
{
    Main,
    Hidden
};
enum class PlayerAnim
{
    Idle,
    Run,
    Jump,
    Crouch,
    Dash,
    Attack
};
enum class BossState
{
    Idle,
    Charge,
    Shoot
};

/* ---------------------------------------------------
   STRUCTS
--------------------------------------------------- */
struct Tile
{
    float x{}, y{};
    TileType type{};
    bool active{true};
    float minX{}, maxX{}, speed{};
    int dir{1};
    float fallTimer{0.0f};
    bool shaking{false}, falling{false};
    float originalX{}, originalY{};
};

struct Particle
{
    float x, y, vx, vy, life;
    SDL_Color color;
    bool isDebris = false;
};

struct Projectile
{
    float x, y, vx, vy;
    bool active;
    bool fromPlayer;
};

struct Enemy
{
    float x, y, vx, vy;
    float startX;
    int w = TILE_WORLD, h = TILE_WORLD;
    TileType type;
    bool active = true;
    float timer = 0.0f;
    int hp = 1;
    bool facingRight = false;
};

struct Boss
{
    float x, y, vx, vy;
    int w = 64, h = 64;
    int hp = BOSS_HP_MAX;
    BossState state = BossState::Idle;
    float stateTimer = 0.0f;
    bool active = false;
};

struct Player
{
    float x{}, y{};
    float vx{}, vy{};
    int w{TILE_WORLD}, h{TILE_WORLD};
    int hp = 3;

    bool onGround{false};
    int jumpsLeft{2};
    bool facingRight{true};

    bool isCrouching{false};
    bool isDashing{false};
    float dashTimer{0.0f};
    float dashCooldown{0.0f};

    float attackTimer{0.0f};
    float meleeVisualTimer{0.0f};
    float shootTimer{0.0f};

    PlayerAnim anim{PlayerAnim::Idle};
    int animFrame{0};
    Uint32 lastAnimTick{0};
};

struct HiddenArea
{
    std::vector<Tile> tiles;
    std::vector<Enemy> enemies;
    SDL_Rect bounds{};
};

struct Level
{
    std::string name;
    std::vector<Tile> tiles;
    std::vector<Enemy> enemies;
    std::vector<HiddenArea> hiddenAreas;
    Boss boss;
    bool isDark{false};
};

struct SaveData
{
    int totalDeaths = 0;
    int highestLevel = 0;
    bool gameCompleted = false;
};

/* ---------------------------------------------------
   GLOBALS
--------------------------------------------------- */
GameState currentState = GameState::Menu;
std::vector<Level> levels;
int currentLevelIndex = 0;
MapContext currentContext = MapContext::Main;
HiddenArea *activeHidden = nullptr;

Player player;
std::vector<Particle> particles;
std::vector<Projectile> projectiles;
SaveData saveData;

float cameraX = 0, cameraY = 0;
float fadeAlpha = 0.0f;
bool fadingOut = false;
int nextLevelTarget = -1;
float checkpointX = 0, checkpointY = 0;
float cameraShakeTimer = 0.0f;

// Settings
bool enableLighting = true;
int menuSelection = 0;

/* ---------------------------------------------------
   UTILS
--------------------------------------------------- */
SDL_Rect getRect(float x, float y, int w, int h)
{
    return {(int)std::round(x), (int)std::round(y), w, h};
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
    return SDL_HasIntersection(&a, &b);
}

// Spawns standard sparks/blood/dust
void spawnParticles(float x, float y, int count, SDL_Color color)
{
    for (int i = 0; i < count; i++)
    {
        Particle p;
        p.x = x;
        p.y = y;
        p.vx = (rand() % 10 - 5) * 0.5f;
        p.vy = (rand() % 10 - 5) * 0.5f - 1.0f;
        p.life = 1.0f;
        p.color = color;
        p.isDebris = false;
        particles.push_back(p);
    }
}

// Spawns breaking block chunks (Gravity affected)
void spawnDebris(float x, float y)
{
    for (int i = 0; i < 4; i++)
    {
        Particle p;
        p.x = x + TILE_WORLD / 2;
        p.y = y + TILE_WORLD / 2;
        p.vx = (i % 2 == 0 ? -1 : 1) * (rand() % 3 + 2);
        p.vy = (i < 2 ? -8.0f : -4.0f);
        p.life = 2.0f;
        p.color = {100, 100, 100, 255}; // Grey debris
        p.isDebris = true;
        particles.push_back(p);
    }
}

TileType parseType(const std::string &name)
{
    if (name == "Spring")
        return TileType::Spring;
    if (name == "EnemyPatrol")
        return TileType::EnemyPatrol;
    if (name == "EnemyFly")
        return TileType::EnemyFly;
    if (name == "EnemyShield")
        return TileType::EnemyShield;
    if (name == "Turret")
        return TileType::Turret;
    if (name == "Boss")
        return TileType::Boss;
    if (name == "Spikes")
        return TileType::Spike;
    if (name == "Checkpoints")
        return TileType::Checkpoint;
    if (name == "Fire")
        return TileType::Fire;
    if (name == "Keys")
        return TileType::Key;
    if (name == "Breakable")
        return TileType::Breakable;
    if (name == "LevelEnd")
        return TileType::LevelEnd;
    if (name == "MovingPlatforms")
        return TileType::MovingPlatform;
    if (name == "FallingPlatform")
        return TileType::FallingPlatform;
    return TileType::Ground;
}

SDL_Rect getSrcRect(TileType type)
{
    int idx = 1;
    switch (type)
    {
    case TileType::Spring:
        idx = 8;
        break;
    case TileType::Spike:
        idx = 68;
        break;
    case TileType::Checkpoint:
        idx = 50;
        break;
    case TileType::Fire:
        idx = 54 + (SDL_GetTicks() / ANIM_SPEED_FIRE % 2);
        break;
    case TileType::Key:
        idx = 86;
        break;
    case TileType::Breakable:
        idx = 21;
        break;
    case TileType::LevelEnd:
        idx = 52;
        break;
    case TileType::MovingPlatform:
        idx = 13;
        break;
    case TileType::FallingPlatform:
        idx = 14;
        break;
    case TileType::EnemyPatrol:
        idx = 67;
        break;
    case TileType::EnemyFly:
        idx = 79;
        break;
    case TileType::EnemyShield:
        idx = 82;
        break;
    case TileType::Turret:
        idx = 80;
        break;
    case TileType::Boss:
        idx = 88;
        break;
    default:
        idx = 1;
        break;
    }
    return {(idx % TILESET_COLUMNS) * TILESET_TILE_SIZE, (idx / TILESET_COLUMNS) * TILESET_TILE_SIZE, TILESET_TILE_SIZE, TILESET_TILE_SIZE};
}

void saveGame()
{
    json j;
    j["totalDeaths"] = saveData.totalDeaths;
    j["highestLevel"] = saveData.highestLevel;
    j["gameCompleted"] = saveData.gameCompleted;
    std::ofstream o("savegame.json");
    o << j;
}

void loadSave()
{
    std::ifstream i("savegame.json");
    if (i.is_open())
    {
        json j;
        i >> j;
        saveData.totalDeaths = j.value("totalDeaths", 0);
        saveData.highestLevel = j.value("highestLevel", 0);
        saveData.gameCompleted = j.value("gameCompleted", false);
    }
}

std::vector<Level> parseMap(const std::string &path)
{
    std::vector<Level> lvls;
    std::ifstream f(path);
    if (!f.is_open())
        return lvls;
    json j;
    f >> j;

    for (auto &lj : j["levels"])
    {
        Level lvl;
        lvl.name = lj["name"];
        if (lj.contains("isDark"))
            lvl.isDark = lj["isDark"];

        for (auto &layer : lj["layers"])
        {
            bool isHidden = (layer["name"] == "HiddenArea");
            HiddenArea ha;
            if (isHidden)
            {
                auto b = layer["bounds"];
                ha.bounds = {b["x"].get<int>() * TILE_WORLD, b["y"].get<int>() * TILE_WORLD, b["w"].get<int>() * TILE_WORLD, b["h"].get<int>() * TILE_WORLD};
            }

            for (auto &t : layer["tiles"])
            {
                float x = t["x"].get<float>() * TILE_WORLD;
                float y = t["y"].get<float>() * TILE_WORLD;
                std::string typeStr = t.contains("type") ? t["type"] : (layer["name"] == "Entities" ? "Entity" : "Surface");
                if (!t.contains("type") && layer["name"] != "Surface" && layer["name"] != "Entities" && !isHidden)
                    typeStr = layer["name"];

                TileType type = parseType(typeStr);

                if (type == TileType::Boss)
                {
                    lvl.boss.active = true;
                    lvl.boss.x = x;
                    lvl.boss.y = y;
                    lvl.boss.hp = BOSS_HP_MAX;
                }
                else if (type == TileType::EnemyPatrol || type == TileType::EnemyFly || type == TileType::EnemyShield || type == TileType::Turret)
                {
                    Enemy e{x, y, 0, 0, x, TILE_WORLD, TILE_WORLD, type};
                    if (isHidden)
                        ha.enemies.push_back(e);
                    else
                        lvl.enemies.push_back(e);
                }
                else
                {
                    Tile tile{x, y, type};
                    tile.originalX = x;
                    tile.originalY = y;
                    if (type == TileType::MovingPlatform)
                    {
                        tile.minX = t["minX"];
                        tile.maxX = t["maxX"];
                        tile.speed = t["speed"];
                    }
                    if (isHidden)
                        ha.tiles.push_back(tile);
                    else
                        lvl.tiles.push_back(tile);
                }
            }
            if (isHidden)
                lvl.hiddenAreas.push_back(ha);
        }
        lvls.push_back(lvl);
    }
    return lvls;
}

/* ---------------------------------------------------
   LOGIC
--------------------------------------------------- */
void resetLevel(int idx)
{
    if (levels.empty())
        return;

    if (idx >= levels.size())
    {
        saveData.gameCompleted = true;
        saveGame();
        currentState = GameState::Win;
        return;
    }

    currentLevelIndex = idx;
    currentContext = MapContext::Main;
    activeHidden = nullptr;
    projectiles.clear();
    particles.clear();

    Level &lvl = levels[idx];
    for (auto &e : lvl.enemies)
    {
        e.active = true;
        e.x = e.startX;
        e.hp = 1;
    }
    lvl.boss.active = (lvl.boss.hp > 0);
    if (lvl.boss.active)
    {
        lvl.boss.hp = BOSS_HP_MAX;
        lvl.boss.state = BossState::Idle;
    }

    for (auto &t : lvl.tiles)
    {
        if (t.type == TileType::Key)
            t.active = true;
        if (t.type == TileType::FallingPlatform)
        {
            t.active = true;
            t.y = t.originalY;
            t.falling = false;
            t.shaking = false;
        }
        if (t.type == TileType::Breakable)
            t.active = true;
    }

    player.x = 2 * TILE_WORLD;
    player.y = 10 * TILE_WORLD;
    player.vx = 0;
    player.vy = 0;
    player.hp = 3;
    checkpointX = player.x;
    checkpointY = player.y;
    cameraX = player.x;
    cameraY = player.y;

    if (idx > saveData.highestLevel)
        saveData.highestLevel = idx;
    saveGame();
}

void triggerTransition(int nextIdx)
{
    fadingOut = true;
    nextLevelTarget = nextIdx;
}

void respawn()
{
    saveData.totalDeaths++;
    player.x = checkpointX;
    player.y = checkpointY;
    player.vx = 0;
    player.vy = 0;
    player.hp = 3;
    currentContext = MapContext::Main;
    activeHidden = nullptr;
    spawnParticles(player.x, player.y, 15, {255, 50, 50, 255});
    fadeAlpha = 255.0f;
    fadingOut = false;
    cameraShakeTimer = 0.5f;
}

void damageEnemy(Enemy &e)
{
    e.hp--;
    if (e.hp <= 0)
    {
        e.active = false;
        spawnParticles(e.x, e.y, 10, {255, 100, 0, 255});
    }
    else
        spawnParticles(e.x, e.y, 5, {200, 200, 200, 255});
}

void damagePlayer()
{
    if (player.dashTimer > 0)
        return;
    player.hp--;
    cameraShakeTimer = 0.3f;
    spawnParticles(player.x, player.y, 10, {255, 0, 0, 255});
    if (player.hp <= 0)
        respawn();
}

void updatePhysics(float dt)
{
    const Uint8 *k = SDL_GetKeyboardState(nullptr);

    if (player.dashCooldown > 0)
        player.dashCooldown -= dt;
    if (player.attackTimer > 0)
        player.attackTimer -= dt;
    if (player.meleeVisualTimer > 0)
        player.meleeVisualTimer -= dt;
    if (player.shootTimer > 0)
        player.shootTimer -= dt;

    if (player.isDashing)
    {
        player.dashTimer -= dt;
        if (player.dashTimer <= 0)
        {
            player.isDashing = false;
            player.vx = 0;
        }
        else
        {
            spawnParticles(player.x + player.w / 2, player.y + player.h / 2, 1, {100, 200, 255, 100});
            return;
        }
    }

    float speed = MOVE_SPEED;
    player.isCrouching = k[SDL_SCANCODE_S] || k[SDL_SCANCODE_DOWN];
    player.h = player.isCrouching ? TILE_WORLD / 2 : TILE_WORLD;
    if (player.isCrouching)
        speed *= 0.5f;

    player.vx = 0;

    if (k[SDL_SCANCODE_LEFT])
    {
        player.vx = -speed;
        player.facingRight = false;
    }
    if (k[SDL_SCANCODE_RIGHT])
    {
        player.vx = speed;
        player.facingRight = true;
    }
    player.vy += GRAVITY;

    Level &lvl = levels[currentLevelIndex];
    auto &tiles = (currentContext == MapContext::Main) ? lvl.tiles : activeHidden->tiles;

    for (auto &t : tiles)
    {
        if (!t.active)
            continue;
        if (t.type == TileType::MovingPlatform)
        {
            t.x += t.speed * t.dir;
            if (t.x >= t.maxX)
                t.dir = -1;
            if (t.x <= t.minX)
                t.dir = 1;
        }
        if (t.type == TileType::FallingPlatform && t.shaking)
        {
            t.fallTimer += dt;
            t.x = t.originalX + (rand() % 3 - 1);
            if (t.fallTimer > 0.5f)
            {
                t.falling = true;
                t.shaking = false;
            }
        }
        if (t.type == TileType::FallingPlatform && t.falling)
        {
            t.y += 5.0f;
            if (t.y > lvl.tiles.back().y + 500)
                t.active = false;
        }
    }

    player.x += player.vx;
    SDL_Rect pRect = getRect(player.x, player.y, player.w, player.h);
    for (auto &t : tiles)
    {
        if (!t.active || t.type == TileType::Key || t.type == TileType::Checkpoint || t.type == TileType::Spike || t.type == TileType::Fire || t.type == TileType::Spring || t.type == TileType::LevelEnd)
            continue;
        if (t.type == TileType::FallingPlatform && t.falling)
            continue;
        SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);
        if (checkCollision(pRect, tRect))
        {
            if (player.vx > 0)
                player.x = tRect.x - player.w;
            if (player.vx < 0)
                player.x = tRect.x + tRect.w;
            player.vx = 0;
        }
    }

    player.y += player.vy;
    player.onGround = false;
    pRect = getRect(player.x, player.y, player.w, player.h);
    for (auto &t : tiles)
    {
        if (!t.active)
            continue;
        SDL_Rect tRect = getRect(t.x, t.y, TILE_WORLD, TILE_WORLD);

        if (t.type == TileType::Spring)
        {
            SDL_Rect sRect = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
            if (checkCollision(pRect, sRect) && player.vy > 0)
            {
                player.vy = -30.0f;
                player.jumpsLeft = 2;
                spawnParticles(player.x, player.y + player.h, 8, {50, 255, 50, 255});
            }
            continue;
        }
        if (t.type == TileType::Spike || t.type == TileType::Fire)
        {
            SDL_Rect hurtBox = {tRect.x + 4, tRect.y + 4, tRect.w - 8, tRect.h - 8};
            if (checkCollision(pRect, hurtBox))
                damagePlayer();
            continue;
        }
        if (t.type == TileType::LevelEnd)
        {
            if (checkCollision(pRect, tRect))
            {
                if (currentContext == MapContext::Hidden)
                {
                    currentContext = MapContext::Main;
                    activeHidden = nullptr;
                    player.x = checkpointX;
                    player.y = checkpointY;
                }
                else
                {
                    triggerTransition(currentLevelIndex + 1);
                }
            }
            continue;
        }
        if (t.type == TileType::Checkpoint)
        {
            if (checkCollision(pRect, tRect))
            {
                checkpointX = t.x;
                checkpointY = t.y;
            }
            continue;
        }
        if (t.type == TileType::MovingPlatform)
        {
            if (player.x + player.w > t.x && player.x < t.x + TILE_WORLD && player.y + player.h >= t.y - 4 && player.y + player.h <= t.y + 10 && player.vy >= 0)
            {
                player.y = t.y - player.h;
                player.vy = 0;
                player.onGround = true;
                player.jumpsLeft = 2;
                player.x += t.speed * t.dir;
                continue;
            }
        }
        if (t.type == TileType::FallingPlatform && t.falling)
            continue;

        if (checkCollision(pRect, tRect))
        {
            if (t.type == TileType::FallingPlatform && player.vy > 0)
                t.shaking = true;
            if (player.vy > 0)
            {
                player.y = tRect.y - player.h;
                player.vy = 0;
                player.onGround = true;
                player.jumpsLeft = 2;
            }
            else if (player.vy < 0)
            {
                player.y = tRect.y + tRect.h;
                player.vy = 0;
                if (t.type == TileType::Breakable)
                {
                    t.active = false;
                    spawnDebris(t.x, t.y);
                    if (!lvl.hiddenAreas.empty())
                    {
                        currentContext = MapContext::Hidden;
                        activeHidden = &lvl.hiddenAreas[0];
                        player.x = activeHidden->bounds.x + 64;
                        player.y = activeHidden->bounds.y + 64;
                    }
                }
            }
        }
    }

    if (player.y > 2000)
        respawn();

    auto &enemies = (currentContext == MapContext::Main) ? lvl.enemies : activeHidden->enemies;
    pRect = getRect(player.x, player.y, player.w, player.h);

    if (player.meleeVisualTimer > 0)
    {
        SDL_Rect attackBox = {(int)(player.x + (player.facingRight ? player.w : -MELEE_RANGE)), (int)player.y, (int)MELEE_RANGE, player.h};
        for (auto &e : enemies)
        {
            if (!e.active)
                continue;
            if (checkCollision(attackBox, getRect(e.x, e.y, e.w, e.h)))
            {
                bool blocked = false;
                if (e.type == TileType::EnemyShield)
                {
                    bool hitFromLeft = player.x < e.x;
                    bool enemyFacingLeft = e.vx < 0;
                    if (hitFromLeft == enemyFacingLeft)
                        blocked = true;
                }
                if (blocked)
                    spawnParticles(e.x, e.y, 2, {100, 100, 255, 255});
                else
                    damageEnemy(e);
            }
        }
        if (lvl.boss.active && checkCollision(attackBox, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
        {
            lvl.boss.hp--;
            spawnParticles(lvl.boss.x + lvl.boss.w / 2, lvl.boss.y + lvl.boss.h / 2, 5, {255, 0, 0, 255});
            if (lvl.boss.hp <= 0)
            {
                lvl.boss.active = false;
                triggerTransition(currentLevelIndex + 1);
            }
        }
    }

    for (auto &e : enemies)
    {
        if (!e.active)
            continue;
        SDL_Rect eRect = getRect(e.x, e.y, e.w, e.h);

        if (e.type == TileType::EnemyPatrol || e.type == TileType::EnemyShield)
        {
            if (e.vx == 0)
                e.vx = ENEMY_SPEED;
            e.x += e.vx;
            if (e.x > e.startX + 64)
                e.vx = -ENEMY_SPEED;
            if (e.x < e.startX - 64)
                e.vx = ENEMY_SPEED;
            if (checkCollision(pRect, eRect))
                damagePlayer();
        }
        else if (e.type == TileType::EnemyFly)
        {
            float dist = std::sqrt(std::pow(player.x - e.x, 2) + std::pow(player.y - e.y, 2));
            if (dist < 300.0f)
            {
                float angle = std::atan2(player.y - e.y, player.x - e.x);
                e.x += std::cos(angle) * FLY_SPEED * 0.5f;
                e.y += std::sin(angle) * FLY_SPEED * 0.5f;
            }
            if (checkCollision(pRect, eRect))
                damagePlayer();
        }
        else if (e.type == TileType::Turret)
        {
            e.timer += dt;
            if (e.timer > TURRET_RATE)
            {
                e.timer = 0;
                Projectile p;
                p.x = e.x;
                p.y = e.y + 8;
                p.vx = (player.x < e.x) ? -4.0f : 4.0f;
                p.vy = 0;
                p.active = true;
                p.fromPlayer = false;
                projectiles.push_back(p);
            }
        }
    }

    if (lvl.boss.active)
    {
        Boss &b = lvl.boss;
        SDL_Rect bRect = getRect(b.x, b.y, b.w, b.h);
        if (checkCollision(pRect, bRect))
            damagePlayer();

        b.stateTimer += dt;
        switch (b.state)
        {
        case BossState::Idle:
            if (b.stateTimer > 2.0f)
            {
                b.state = BossState::Charge;
                b.stateTimer = 0;
            }
            break;
        case BossState::Charge:
            b.x += (player.x < b.x ? -BOSS_SPEED : BOSS_SPEED);
            if (b.stateTimer > 2.0f)
            {
                b.state = BossState::Shoot;
                b.stateTimer = 0;
            }
            break;
        case BossState::Shoot:
            if ((int)(b.stateTimer * 10) % 5 == 0)
            {
                Projectile p;
                p.x = b.x + b.w / 2;
                p.y = b.y + b.h / 2;
                p.vx = (rand() % 10 - 5);
                p.vy = (rand() % 10 - 5);
                p.active = true;
                p.fromPlayer = false;
                projectiles.push_back(p);
            }
            if (b.stateTimer > 2.0f)
            {
                b.state = BossState::Idle;
                b.stateTimer = 0;
            }
            break;
        }
    }

    for (auto &p : projectiles)
    {
        if (!p.active)
            continue;
        p.x += p.vx;
        p.y += p.vy;
        SDL_Rect r = getRect(p.x, p.y, 8, 8);
        if (p.fromPlayer)
        {
            for (auto &e : enemies)
            {
                if (e.active && checkCollision(r, getRect(e.x, e.y, e.w, e.h)))
                {
                    if (e.type != TileType::EnemyShield)
                        damageEnemy(e);
                    p.active = false;
                }
            }
            if (lvl.boss.active && checkCollision(r, getRect(lvl.boss.x, lvl.boss.y, lvl.boss.w, lvl.boss.h)))
            {
                lvl.boss.hp--;
                p.active = false;
            }
        }
        else
        {
            if (checkCollision(r, pRect))
            {
                damagePlayer();
                p.active = false;
            }
        }
        if (std::abs(p.x - player.x) > 600)
            p.active = false;
    }

    for (auto it = particles.begin(); it != particles.end();)
    {
        it->x += it->vx;
        it->y += it->vy;
        if (it->isDebris)
            it->vy += 0.5f;
        it->life -= 0.05f;
        if (it->life <= 0)
            it = particles.erase(it);
        else
            ++it;
    }
}

/* ---------------------------------------------------
   MAIN
--------------------------------------------------- */
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Reflex Rift", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    std::string path = "/Users/lazycodebaker/Documents/Dev/CPP/platformer_2d_temp/assets/";
    SDL_Texture *tilesTex = IMG_LoadTexture(renderer, (path + "spritesheet_tiles_resized.png").c_str());
    SDL_Texture *playerTex = IMG_LoadTexture(renderer, (path + "spritesheet_characters.png").c_str());
    TTF_Font *font = TTF_OpenFont((path + "PixelifySans-VariableFont_wght.ttf").c_str(), 24);

    // Glow Texture
    SDL_Texture *glowTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 64, 64);
    SDL_SetTextureBlendMode(glowTex, SDL_BLENDMODE_ADD);
    SDL_SetRenderTarget(renderer, glowTex);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            float dist = std::sqrt(std::pow(x - 32, 2) + std::pow(y - 32, 2));
            if (dist < 32)
            {
                int alpha = (int)(255 * (1.0f - dist / 32.0f));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
    SDL_SetRenderTarget(renderer, nullptr);

    // Lighting (Shadow Mask)
    SDL_Texture *lightLayer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetTextureBlendMode(lightLayer, SDL_BLENDMODE_BLEND);
    SDL_Texture *lightMask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 300);
    SDL_BlendMode eraser = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD);
    SDL_SetTextureBlendMode(lightMask, eraser);
    SDL_SetRenderTarget(renderer, lightMask);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int w = 0; w < 300; w++)
        for (int h = 0; h < 300; h++)
            if ((150 - w) * (150 - w) + (150 - h) * (150 - h) < 22500)
                SDL_RenderDrawPoint(renderer, w, h);

    SDL_SetRenderTarget(renderer, nullptr);

    loadSave();
    levels = parseMap(path + "map_3.json");
    if (levels.empty())
    {
        Level errorLevel;
        errorLevel.name = "ERROR: MAP NOT FOUND";
        levels.push_back(errorLevel);
    }

    auto renderTextCentered = [&](std::string text, int y, SDL_Color color, float scale = 1.0f)
    {
        if (!font)
            return;
        SDL_Surface *s = TTF_RenderText_Blended(font, text.c_str(), color);
        if (!s)
            return;
        SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
        int w = (int)(s->w * scale);
        int h = (int)(s->h * scale);
        SDL_Rect dst = {WINDOW_WIDTH / 2 - w / 2, y, w, h};
        SDL_RenderCopy(renderer, t, nullptr, &dst);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    };

    bool running = true;
    Uint32 lastTime = SDL_GetTicks();

    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        float dt = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;
            if (e.type == SDL_KEYDOWN)
            {
                if (currentState == GameState::Menu)
                {
                    if (saveData.gameCompleted)
                    {
                        if (e.key.keysym.scancode == SDL_SCANCODE_UP)
                        {
                            menuSelection--;
                            if (menuSelection < 0)
                                menuSelection = levels.size() - 1;
                        }
                        if (e.key.keysym.scancode == SDL_SCANCODE_DOWN)
                        {
                            menuSelection++;
                            if (menuSelection >= levels.size())
                                menuSelection = 0;
                        }
                        if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
                        {
                            currentState = GameState::Playing;
                            resetLevel(menuSelection);
                        }
                    }
                    else
                    {
                        if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
                        {
                            currentState = GameState::Playing;
                            resetLevel((saveData.highestLevel < levels.size()) ? saveData.highestLevel : 0);
                        }
                    }
                    if (e.key.keysym.scancode == SDL_SCANCODE_L)
                        enableLighting = !enableLighting;
                }
                else if (currentState == GameState::Win)
                {
                    if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
                        currentState = GameState::Menu;
                }
                else if (currentState == GameState::Playing)
                {
                    if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        currentState = GameState::Menu;
                    if (e.key.keysym.scancode == SDL_SCANCODE_SPACE && player.jumpsLeft > 0)
                    {
                        player.vy = JUMP_FORCE;
                        player.jumpsLeft--;
                        player.onGround = false;
                    }
                    if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT && !player.isDashing && player.dashCooldown <= 0)
                    {
                        player.isDashing = true;
                        player.dashTimer = DASH_DURATION;
                        player.dashCooldown = DASH_COOLDOWN;
                        player.vx = player.facingRight ? DASH_SPEED : -DASH_SPEED;
                        player.vy = 0;
                    }
                    if (e.key.keysym.scancode == SDL_SCANCODE_Z && player.attackTimer <= 0)
                    {
                        player.attackTimer = MELEE_COOLDOWN;
                        player.meleeVisualTimer = 0.1f;
                    }
                    if (e.key.keysym.scancode == SDL_SCANCODE_X && player.shootTimer <= 0)
                    {
                        player.shootTimer = SHOOT_COOLDOWN;
                        Projectile p;
                        p.x = player.x + player.w / 2;
                        p.y = player.y + player.h / 2;
                        p.vx = player.facingRight ? PROJECTILE_SPEED : -PROJECTILE_SPEED;
                        p.vy = 0;
                        p.active = true;
                        p.fromPlayer = true;
                        projectiles.push_back(p);
                    }
                }
            }
        }

        if (currentState == GameState::Playing)
        {
            updatePhysics(dt);
            if (fadingOut)
            {
                fadeAlpha += FADE_SPEED * dt;
                if (fadeAlpha >= 255)
                {
                    resetLevel(nextLevelTarget);
                    fadingOut = false;
                }
            }
            else if (fadeAlpha > 0)
                fadeAlpha -= FADE_SPEED * dt;

            if (cameraShakeTimer > 0)
            {
                cameraShakeTimer -= dt;
                cameraX += (rand() % 10 - 5);
                cameraY += (rand() % 10 - 5);
            }
        }

        float targetCamX = player.x - WINDOW_WIDTH / 2;
        float targetCamY = player.y - WINDOW_HEIGHT / 2;
        cameraX += (targetCamX - cameraX) * 0.1f;
        cameraY += (targetCamY - cameraY) * 0.1f;

        SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
        SDL_RenderClear(renderer);

        if (currentState == GameState::Menu)
        {
            int y = 50;
            renderTextCentered("Reflex Rift", y, {255, 255, 255, 255}, 1.5f);
            y += 50;
            renderTextCentered("--- CONTROLS ---", y, {180, 180, 180, 255}, 0.8f);
            y += 25;
            renderTextCentered("Arrows: Move | Space: Jump | Shift: Dash", y, {180, 180, 180, 255}, 0.8f);
            y += 25;
            renderTextCentered("Z: Melee | X: Fireball", y, {180, 180, 180, 255}, 0.8f);
            y += 25;
            renderTextCentered("L: Toggle Lights", y, {180, 180, 180, 255}, 0.8f);
            y += 40;

            if (saveData.gameCompleted)
            {
                renderTextCentered("GAME COMPLETED! SELECT LEVEL:", y, {255, 215, 0, 255});
                y += 40;
                for (int i = 0; i < levels.size(); i++)
                {
                    std::string lvlName = "Level " + std::to_string(i + 1) + ": " + levels[i].name;
                    SDL_Color c = (i == menuSelection) ? SDL_Color{100, 255, 100, 255} : SDL_Color{180, 180, 180, 255};
                    if (i == menuSelection)
                        lvlName = "> " + lvlName + " <";
                    renderTextCentered(lvlName, y, c);
                    y += 30;
                }
            }
            else
            {
                renderTextCentered("Press ENTER to Start", y, {255, 215, 0, 255});
            }
        }
        else if (currentState == GameState::Playing)
        {
            // DARK BACKGROUND (Industrial/Cave Theme)
            SDL_SetRenderDrawColor(renderer, 15, 15, 20, 255);
            SDL_RenderClear(renderer);

            Level &level = levels[currentLevelIndex];
            auto &tiles = (currentContext == MapContext::Main) ? level.tiles : activeHidden->tiles;
            auto &enemies = (currentContext == MapContext::Main) ? level.enemies : activeHidden->enemies;

            for (auto &t : tiles)
            {
                if (!t.active)
                    continue;
                SDL_Rect d = getRect(t.x - cameraX, t.y - cameraY, TILE_WORLD, TILE_WORLD);
                SDL_Rect s = getSrcRect(t.type);
                SDL_RenderCopy(renderer, tilesTex, &s, &d);
            }

            for (auto &t : tiles)
            {
                if (!t.active)
                    continue;
                if (t.type == TileType::LevelEnd)
                {
                    SDL_Rect d = getRect(t.x - cameraX - 16, t.y - cameraY - 16, 64, 64);
                    SDL_SetTextureColorMod(glowTex, 0, 255, 0);
                    SDL_RenderCopy(renderer, glowTex, nullptr, &d);
                }
            }

            for (auto &e : enemies)
            {
                if (!e.active)
                    continue;
                SDL_Rect d = getRect(e.x - cameraX, e.y - cameraY, e.w, e.h);
                SDL_Rect s = getSrcRect(e.type);
                SDL_RenderCopyEx(renderer, tilesTex, &s, &d, 0, nullptr, (e.vx > 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            }
            if (level.boss.active)
            {
                SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
                SDL_Rect bRect = getRect(level.boss.x - cameraX, level.boss.y - cameraY, level.boss.w, level.boss.h);
                SDL_RenderFillRect(renderer, &bRect);
            }

            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
            for (auto &p : projectiles)
            {
                if (!p.active)
                    continue;
                SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 8, 8);
                SDL_RenderFillRect(renderer, &r);
            }
            for (auto &p : particles)
            {
                SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, (Uint8)(p.life * 255));
                SDL_Rect r = getRect(p.x - cameraX, p.y - cameraY, 4, 4);
                SDL_RenderFillRect(renderer, &r);
            }

            int pRow = (!player.onGround) ? 1 : 0;
            if (SDL_GetTicks() - player.lastAnimTick > ANIM_SPEED_PLAYER)
            {
                player.animFrame = (player.animFrame + 1) % 2;
                player.lastAnimTick = SDL_GetTicks();
            }
            SDL_Rect pSrc = {player.animFrame * 16, pRow * 16, 16, 16};
            if (player.isCrouching)
                pSrc.h = 16;
            SDL_Rect pDst = getRect(player.x - cameraX, player.y - cameraY, player.w, player.h);
            SDL_RenderCopyEx(renderer, playerTex, &pSrc, &pDst, 0, nullptr, player.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);

            if (player.meleeVisualTimer > 0)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
                SDL_Rect swing = {(int)(pDst.x + (player.facingRight ? pDst.w : -MELEE_RANGE)), pDst.y, (int)MELEE_RANGE, pDst.h};
                SDL_RenderFillRect(renderer, &swing);
            }

            if (enableLighting && level.isDark)
            {
                SDL_SetRenderTarget(renderer, lightLayer);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 230);
                SDL_RenderClear(renderer);
                SDL_Rect lDst = {(int)(player.x - cameraX + player.w / 2 - 150), (int)(player.y - cameraY + player.h / 2 - 150), 300, 300};
                SDL_RenderCopy(renderer, lightMask, nullptr, &lDst);
                SDL_SetRenderTarget(renderer, nullptr);
                SDL_RenderCopy(renderer, lightLayer, nullptr, nullptr);
            }

            if (font)
            {
                std::string hud = "HP: " + std::to_string(player.hp);
                SDL_Surface *s = TTF_RenderText_Blended(font, hud.c_str(), {255, 255, 255, 255});
                SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
                SDL_Rect d = {20, 20, s->w, s->h};
                SDL_RenderCopy(renderer, t, nullptr, &d);
                SDL_FreeSurface(s);
                SDL_DestroyTexture(t);
            }
            if (fadeAlpha > 0)
            {
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, (Uint8)fadeAlpha);
                SDL_Rect fs = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
                SDL_RenderFillRect(renderer, &fs);
            }
        }
        else if (currentState == GameState::Win)
        {
            renderTextCentered("ALL LEVELS CLEARED!", WINDOW_HEIGHT / 2 - 30, {255, 215, 0, 255}, 2.0f);
            renderTextCentered("Press ENTER to return to Menu", WINDOW_HEIGHT / 2 + 40, {200, 200, 200, 255}, 1.0f);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(glowTex);
    SDL_DestroyTexture(lightLayer);
    SDL_DestroyTexture(lightMask);
    SDL_DestroyTexture(tilesTex);
    SDL_DestroyTexture(playerTex);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}