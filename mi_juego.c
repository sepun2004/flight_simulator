


// Realizar un programa en el que una pelota rote por una circunferencia

#pragma region Includes
    #include <stdio.h>
    #include <math.h>
    #include <stdbool.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_image.h>

#pragma endregion

#pragma region Mis definiciones
    #define NUM_AROS 1
    const int DELAY = 30; // ms
    const int SCREEN_WIDTH = 1024; // 800
    const int SCREEN_HEIGHT = 768; // 600
    const int SCREEN_W_MID = SCREEN_WIDTH / 2;
    const int SCREEN_H_MID = SCREEN_HEIGHT / 2;
    const char TITLE[] = "Practica Sebastian";
    const SDL_Color color_rojo = {255, 0, 0, 255};
    const SDL_Color color_azul = {0, 0, 255, 255};
    const SDL_Color color = {255, 0, 0, 255};
    double size = 2449.2;

    const float PI = 3.1415;
    const float G = 9.81;

    typedef struct Aro
    {
        float x, y, z; // Posición del aro
        int size; // Tamaño de la imagen del aro
        SDL_Texture *texture; // Textura del aro
    } aro_t;

    typedef struct Avion
    {
        float v;         //Velocidad
        float theta;     //Angulo de rotacion (RAD)
        float phi;       //Angulo de elevacion (RAD)
        float x, y, z;   //Posicion en coordenadas cartesianas
    } avion_t;
    int contador = 0;

    void entrada_teclado(SDL_Event event);
    void actualizar_posicion(avion_t *avion);
    void fondo();
    void dibujar_aros();
    bool detectar_colision(avion_t *avion, aro_t *aro);
    void actualizar_posicion_aro(aro_t *aros, avion_t *avion);
    SDL_Texture *cargarImagen(SDL_Renderer *renderer, const char *filePath);
    void dibujar_imagen(SDL_Renderer *renderer, const char *filePath, int x, int y, int width, int height, double angle);
    void pintar_avion();
    void pintar_tierra(avion_t *avion);
    void reinicio(avion_t *avion);
    bool detectar_colision_suelo(avion_t *avion);
#pragma endregion

#pragma region SDL2
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *backgroundTexture = NULL;
    SDL_Texture *sueloTexture = NULL; // Añadir textura del suelo
    TTF_Font *font = NULL;
    bool keys[SDL_NUM_SCANCODES] = {false}; // Array para almacenar el estado de las teclas
    bool initSDL(aro_t *aros);
    void cleanSDL(aro_t *aros);
    void clean_screen();
    void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);
#pragma endregion




int WinMain(int argc, char *argv[])
{
    aro_t aros[NUM_AROS] = {{0, 0, 100, 100, NULL}}; // Inicializar aros
    if (!initSDL(aros))
        return 1;
    bool running = true;
    SDL_Event event;

    #pragma region Mis variables // Variables a manejar dentro del bucle principal
        avion_t avion = {0, 0, 0, 0, 0, 0};
    #pragma endregion

    while (running) //Bucle principal
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            else
                entrada_teclado(event);         
        }
        if (keys[SDL_SCANCODE_UP])
            avion.phi += PI / 180;
        if (keys[SDL_SCANCODE_DOWN])
            avion.phi -= PI / 180;
        if (keys[SDL_SCANCODE_LEFT])
            avion.theta -= PI / 180;
        if (keys[SDL_SCANCODE_RIGHT])
            avion.theta += PI / 180;
        if (keys[SDL_SCANCODE_W])
            avion.v += PI / 180;
        if (keys[SDL_SCANCODE_S])
            avion.v -= PI / 180;
        clean_screen();
        fondo();
        dibujar_aros(aros, &avion);
        for (int i = 0; i < NUM_AROS; i++)
        {
            if (detectar_colision(&avion, &aros[i]))
            {
                printf("Colisión con aro %d\n", i);
                contador++;
                if (contador == NUM_AROS)
                {
                    printf("Ganaste\n");
                    running = false;
                }
            }
        }


        #pragma region Mi código 
            renderText(renderer, font, "Simulador", color_azul, SCREEN_WIDTH/2-50, 50);
            actualizar_posicion(&avion);
            pintar_tierra(&avion);
            pintar_avion();
            if (avion.phi > 2 *(PI))
                avion.phi = 0;
            else if (avion.phi < 0)
                avion.phi = 2 * (PI);
            actualizar_posicion_aro(aros, &avion);
            if (detectar_colision_suelo(&avion))
            {
                printf("Colisión con el suelo\n");
                reinicio(&avion);
            }
            // printf("z = %.2f\n", avion.z);
            char buffer[128];
            sprintf(buffer, "v = %.2f, theta = %.2f, phi = %.2f, x = %.2f, y = %.2f, z = %.2f", avion.v, avion.theta, avion.phi, avion.x, avion.y, avion.z);
            renderText(renderer, font, buffer, color, 50, 100);
        #pragma endregion

        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY); // Aprox. 1000/DELAY FPS      
    }
    cleanSDL(aros);
    return 0;
}

#pragma region Mis definiciones

    void fondo()
    {
        dibujar_imagen(renderer, "img/fondo.jpg", SCREEN_W_MID, SCREEN_H_MID, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    }

    void reinicio(avion_t *avion)
    {
        printf("Colicion con el suelo\n");
        avion->x = 0;
        avion->y = 0;
        avion->z = 0;
        avion->phi = 0;
        avion->theta = 0;
        avion->v = 0;
    }

    void pintar_tierra(avion_t *avion)
    {
        float theta_radianes = avion->theta * PI / 180; // Convertir a radianes
        float distancia = ((avion->phi - (PI)) * SCREEN_HEIGHT); // Distancia desde el avión al suelo
        int x = distancia * sin(theta_radianes + PI); // Calcular la posición x en función de theta
        float tmp_x = SCREEN_W_MID + x; // Ajustar la posición x
        int y = distancia * cos(theta_radianes + PI); // Calcular la posición y en función de theta
        float tmp_y = SCREEN_H_MID + y; // Ajustar la posición y
        int SCREEN_WIDTH_temp = SCREEN_WIDTH * 1.5; // Ajustar el ancho de la imagen
        float agulo = avion->theta * -10; // Ajustar el ángulo de rotación

        if (avion->z == -30) // Si el avión choca con el suelo
            reinicio(avion); // Reiniciar la posición del avión
        dibujar_imagen(renderer, "img/piso.jpg", tmp_x, tmp_y, SCREEN_WIDTH_temp, 2449.2, agulo); // Dibujar el suelo
    }

    SDL_Texture *cargarImagen(SDL_Renderer *renderer, const char *filePath)
    {
        SDL_Surface *imageSurface = IMG_Load(filePath);
        if (imageSurface == NULL)
        {
            printf("Error al cargar la imagen: %s\n", IMG_GetError());
            return NULL;
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
        SDL_FreeSurface(imageSurface);
        if (texture == NULL)
        {
            printf("Error al crear la textura de la imagen: %s\n", SDL_GetError());
        }
        return texture;
    }

    void dibujar_imagen(SDL_Renderer *renderer, const char *filePath, int x, int y, int width, int height, double angle)
    {
        SDL_Texture *texture = cargarImagen(renderer, filePath);
        if (texture == NULL)
            return;
        SDL_Rect dstRect = {x - (width / 2), y - (height / 2), width, height};
        SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, angle, NULL, SDL_FLIP_NONE);
        SDL_DestroyTexture(texture);
    }

    void actualizar_posicion(avion_t *avion)
    {
        avion->x += avion->v * sin(avion->theta) * cos(avion->phi);
        avion->y += avion->v * sin(avion->theta) * cos(avion->phi);
        avion->z += avion->v * cos(avion->phi);
    }

    void entrada_teclado(SDL_Event event)
    {
        if (event.type == SDL_KEYDOWN)
            keys[event.key.keysym.scancode] = true; // Actualiza el estado de la tecla presionada
        else if (event.type == SDL_KEYUP)
            keys[event.key.keysym.scancode] = false; // Actualiza el estado de la tecla liberada
    }

    void dibujar_aros(aro_t *aros, avion_t *avion)
    {
        for (int i = 0; i < NUM_AROS; i++)
        {
            if (aros[i].texture == NULL)
            {
                printf("Error  %d: %s\n", i, IMG_GetError());
                continue;
            }
             // Verificar si el avión está mirando hacia atrás
            if (avion->phi > PI / 2 && avion->phi < 3 * PI / 2)
                continue; // No dibujar el aro si el avión está mirando hacia atrás
            float scale = 1.0f / (1.0f + aros[i].z / 100.0f);
            int x = SCREEN_WIDTH / 2 + (aros[i].x - avion->x) * scale;
            int y = SCREEN_HEIGHT / 2 - (aros[i].y - avion->y) * scale;
            int size = aros[i].size; // Usar el tamaño calculado en actualizar_posicion_aro

            // Ajustar la posición en función de la orientación del avión
            float desplazamientoY = tan(avion->phi) * SCREEN_HEIGHT / 2;
            y += desplazamientoY;
            

            SDL_Rect rect = {x - size / 2, y - size / 2, size, size};
            SDL_RenderCopy(renderer, aros[i].texture, NULL, &rect);
        }
    }

    bool detectar_colision_suelo(avion_t *avion)
    {
        // Considerar una colisión si el avión está cerca del suelo
        return avion->z < -30.0f; // Ajusta este valor según sea necesario
    }

    bool detectar_colision(avion_t *avion, aro_t *aro)
    {
        // Considerar una colisión si las coordenadas del avión están cerca de las del aro
        float distancia = sqrt(pow(avion->x - aro->x, 2) + pow(avion->y - aro->y, 2) + pow(avion->z - aro->z, 2));
        return distancia < 10.0f; // Ajusta este valor según sea necesario
    }

    void actualizar_posicion_aro(aro_t *aros, avion_t *avion)
    {
        for (int i = 0; i < NUM_AROS; i++)
        {
            // Calcular la distancia entre el avión y el aro
            float distancia = sqrt(pow(avion->x - aros[i].x, 2) + pow(avion->y - aros[i].y, 2) + pow(avion->z - aros[i].z, 2));

            int size;
            float scale = 100.0f / distancia;
            size = 100 * scale;

            // Actualizar el tamaño del aro
            aros[i].size = size;

            // Ajustar la posición del aro en función de theta
            if (avion->theta > 0 && avion->theta < PI)
            {
                // Si el avión se mueve a la derecha, mover el aro a la izquierda
                aros[i].x -= cos(avion->theta) * 50; // Ajusta el valor 50 según sea necesario
                aros[i].y -= sin(avion->theta) * 50; // Ajusta el valor 50 según sea necesario
            }
            else if (avion->theta > PI && avion->theta < 2 * PI)
            {
                // Si el avión se mueve a la izquierda, mover el aro a la derecha
                aros[i].x += cos(avion->theta) * 50; // Ajusta el valor 50 según sea necesario
                aros[i].y += sin(avion->theta) * 50; // Ajusta el valor 50 según sea necesario
            }
        }
    }
    
    void pintar_avion() // simulando el vuelo del avion es decir que si vamos para la derecha la imagen debe rotar
    {
        dibujar_imagen(renderer, "img/cabina2.png", SCREEN_W_MID, SCREEN_H_MID, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    }



#pragma endregion

#pragma region SDL
    bool initSDL(aro_t *aros)
    {
        // Inicialización de SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("Error al inicializar SDL: %s\n", SDL_GetError());
            return false;
        }

        // Inicialización de SDL_ttf
        if (TTF_Init() == -1)
        {
            printf("Error al inicializar SDL_ttf: %s\n", TTF_GetError());
            SDL_Quit();
            return false;
        }

        // Inicialización de SDL_image
        if (IMG_Init(IMG_INIT_PNG) < 0)
        {
            printf("Error al inicializar SDL_image: %s\n", IMG_GetError());
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        // Crear ventana
        window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
        {
            printf("Error al crear la ventana: %s\n", SDL_GetError());
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        // Crear renderer
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL)
        {
            printf("Error al crear el renderer: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        // Cargar fuente
        font = TTF_OpenFont("arial/arial.ttf", 24);
        if (font == NULL)
        {
            printf("Error al cargar la fuente: %s\n", TTF_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        // Cargar textura de aro
        SDL_Surface *surface = IMG_Load("img/aros.png");
        if (!surface)
        {
            printf("Error al cargar la imagen del aro: %s\n", IMG_GetError());
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return false;
        }

        for (int i = 0; i < NUM_AROS; i++)
        {
            aros->texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!aros->texture)
            {
                printf("Error al crear la textura del aro  %d: %s\n", i, SDL_GetError());
                TTF_CloseFont(font);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                IMG_Quit();
                TTF_Quit();
                SDL_Quit();
                return false;
            }
            i++;
        }

        // Cargar textura del suelo
        surface = IMG_Load("img/piso.jpg");
        if (!surface)
        {
            printf("Error al cargar la imagen del suelo: %s\n", IMG_GetError());
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return false;
        }
        sueloTexture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!sueloTexture)
        {
            printf("Error al crear la textura del suelo: %s\n", SDL_GetError());
            TTF_CloseFont(font);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            TTF_Quit();
            SDL_Quit();
            return false;
        }
        SDL_FreeSurface(surface); // Libera la superficie después de crear las texturas

        return true;
    }

    void cleanSDL(aro_t *aros)
    {
        if (backgroundTexture)
        {
            SDL_DestroyTexture(backgroundTexture);
        }
        if (sueloTexture)
        {
            SDL_DestroyTexture(sueloTexture);
        }
        for (int i = 0; i < NUM_AROS; i++)
        {
            if (aros[i].texture)
            {
                SDL_DestroyTexture(aros[i].texture);
            }
        }
        if (renderer)
        {
            SDL_DestroyRenderer(renderer);
        }
        if (window)
        {
            SDL_DestroyWindow(window);
        }
        TTF_Quit(); //agregado
        IMG_Quit(); //agregado
        SDL_Quit();
    }

    void clean_screen()
    {
        // Limpiar la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fondo negro
        SDL_RenderClear(renderer);
    }

    void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y)
    {
        SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, color); // Soporta transparencias
        if (!textSurface)
        {
            printf("Error creando la superficie del texto: %s\n", TTF_GetError());
            return;
        }
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
        SDL_FreeSurface(textSurface); // Liberar la superficie después de crear la textura
        if (!textTexture)
        {
            printf("Error creando la textura del texto: %s\n", SDL_GetError());
            return;
        }
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // Renderizar texto
        SDL_DestroyTexture(textTexture); // Liberar textura después de renderizar
    }

#pragma endregion