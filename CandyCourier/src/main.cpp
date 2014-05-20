#include <stdio.h>
#include <oxygarum.h>

#include "chunk.h"
#include "materials.h"
#include "player.h"

using namespace oxygarum;

Player *player;

void quit(SDL_Event *e)
{
	printf("Exiting..\n");
	exit(0);
}

void gen_texcoords(Vector2D *texcoords, int x, int y, int gx, int gy)
{
	Vector2D loc = Vector2D(
		(float)x / (float)gx,
		1.0f - ((float)y / (float)gy)
	);
	Vector2D size = Vector2D(
		1.0f / (float)gx,
		1.0f / (float)gy
	);

	texcoords[3] = Vector2D(loc.x +1.0f/64.0f,loc.y - size.y);
	texcoords[2] = Vector2D(loc.x + size.x, loc.y - size.y);
	texcoords[1] = Vector2D(loc.x + size.x, loc.y);
	texcoords[0] = Vector2D(loc.x +1.0f/64.0f, loc.y);
}

void keyboard_down(SDL_Event *e)
{
	switch(e->key.keysym.sym)
	{
		case 'a':
			player->move(-1);
			break;
		case 'd':
			player->move(1);
			break;
		case ' ':
		case 's':
			player->jump(2);
			break;
	}
}

void keyboard_up(SDL_Event *e)
{
	if(e->key.keysym.sym == 'a' ||
	   e->key.keysym.sym == 'd')
	{
		player->move(0);
	}
}

int main(void)
{
	printf("Hello Candy-World!\n");

	SDLWindow *window = new SDLWindow("CandyCourier", 800,600);
	EventManager *eventmanager = new EventManager();

	eventmanager->register_handler(SDL_QUIT, quit);
	eventmanager->register_handler(SDL_KEYDOWN, keyboard_down);
	eventmanager->register_handler(SDL_KEYUP, keyboard_up);

	player = new Player();

	Scene *scene = new Scene();
	Camera *camera = new Camera(window, scene);
	camera->position = Vector3D(0.0f, -8.0f, -25.0f);

	Chunk *testchunk = new Chunk();

	int x,y;
	for(x = 0; x < CHUNK_SIZE_X; x++)
	{
		for(y = 0; y < CHUNK_SIZE_Y; y++)
		{
			if(y < x)
			{
				testchunk->blocks[x][y] = STONE;
			}
			else if(y < x+5)
			{
				testchunk->blocks[x][y] = DIRT;
			}
			else if(y < x+6)
			{
				testchunk->blocks[x][y] = GRASS;
			}
			else
			{
				testchunk->blocks[x][y] = NONE;
			}
		}
	}

	Material *mat = new Material();
	Texture *tex = loader::load_texture("data/texture.png");
	tex->params->add(new TextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	tex->params->add(new TextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	tex->load();
	mat->map_texture(tex, "", 0);

	testchunk->generate_mesh();
	testchunk->obj->material = mat;
	
	player->obj->material = mat;

	scene->objects3D->add(testchunk->obj);
	scene->objects3D->add(player->obj);

	while(1)
	{
		// handle events
		eventmanager->poll_events();

		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->render();

		// update
		float frametime = window->update();
		player->update(frametime*0.005, &testchunk);
		camera->position.x = - player->obj->position.x;
	}

	return 0;
}

