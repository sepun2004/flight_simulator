# ✈️ Simulador de Vuelo en SDL2

Este es un simulador de vuelo desarrollado en **C** utilizando **SDL2**. El programa permite controlar un avión en un entorno 2D, con detección de colisiones y renderizado de elementos como el suelo y los aros de paso.

## 🚀 Características
- Simulación de movimiento del avión con controles de teclado.
- Detección de colisión con el suelo y reinicio de la simulación.
- Renderizado de aros con perspectiva.
- Física simplificada para el vuelo.

---

## 🖥️ Instalación y Ejecución
### Requisitos:
- **SDL2**
- **SDL2_ttf**
- **SDL2_image**
- Un compilador compatible con C

### 🔧 Compilación:
```bash
gcc main.c -o simulador -lSDL2 -lSDL2_ttf -lSDL2_image -lm
```

## Controles

| Tecla | Acción |
|-------|--------|
| ⬆️ (Up) | Elevar el avión |
| ⬇️ (Down) | Descender el avión |
| ⬅️ (Left) | Girar a la izquierda |
| ➡️ (Right) | Girar a la derecha |
| W | Aumentar velocidad |
| S | Disminuir velocidad |

## Función `pintar_tierra`
Dibuja la imagen del suelo ajustando su posición y ángulo en función de la inclinación del avión.
```c
void pintar_tierra(avion_t *avion) {
    float theta_radianes = avion->theta * PI / 180;
    float distancia = ((avion->yphi - PI) * SCREEN_HEIGHT);
    int x = distancia * sin(theta_radianes + PI);
    float tmp_x = SCREEN_W_MID + x;
    int y = distancia * cos(theta_radianes + PI);
    float tmp_y = SCREEN_H_MID + y;
    int SCREEN_WIDTH_temp = SCREEN_WIDTH * 1.5;
    float angulo = avion->theta * -10;

    if (avion->xz == -30)
        reinicio(avion);

    dibujar_imagen(renderer, "img/piso.jpg", tmp_x, tmp_y, SCREEN_WIDTH_temp, 2449.2, angulo);
}
```
### Explicacion :
- Calcula la posición del suelo en función del ángulo de inclinación (theta).
- Ajusta la imagen del suelo para simular profundidad.
- Reinicia el avión si choca con el suelo.

## Funcion `entrada_teclado`
Registra las teclas presionadas y almacenadas en un array keys[] para gestionar las entradas del usuario.
```c
    void entrada_teclado(SDL_Event event)
    {
        if (event.type == SDL_KEYDOWN)
            keys[event.key.keysym.scancode] = true; // Marca la tecla como presionada
        else if (event.type == SDL_KEYUP)
            keys[event.key.keysym.scancode] = false; // Marca la tecla como liberada
    }

```
### Explicacion :
- Detecta eventos de teclado y almacena su estado en keys[].
- Esto permite comprobar en el bucle principal si una tecla está siendo presionada sin necesidad de detectar eventos en cada iteración.


## Funcion `dibujar_aros`
Dibuja los aros en pantalla, ajustando su tamaño y posición en función de la perspectiva del avión.
```c
    void dibujar_aros(aro_t *aros, avion_t *avion)
    {
        for (int i = 0; i < NUM_AROS; i++)
        {
            if (aros[i].texture == NULL)
            {
                printf("Error  %d: %s\n", i, IMG_GetError());
                continue;
            }
            
            if (avion->phi > PI / 2 && avion->phi < 3 * PI / 2)
                continue; // No dibujar si el avión está mirando hacia atrás
    
            float scale = 1.0f / (1.0f + aros[i].z / 100.0f);
            int x = SCREEN_WIDTH / 2 + (aros[i].x - avion->x) * scale;
            int y = SCREEN_HEIGHT / 2 - (aros[i].y - avion->y) * scale;
            int size = aros[i].size;
    
            float desplazamientoY = tan(avion->phi) * SCREEN_HEIGHT / 2;
            y += desplazamientoY;
            
            SDL_Rect rect = {x - size / 2, y - size / 2, size, size};
            SDL_RenderCopy(renderer, aros[i].texture, NULL, &rect);
        }
    }
```
### Explicacion :
- Ajusta la escala de los aros según la distancia del avión.
- No muestra los aros si el avión está mirando hacia atrás.
- Usa SDL_RenderCopy() para renderizar la textura en la pantalla.


### 📷 Capturas de Pantalla


<p align="center">
  <img src="https://github.com/sepun2004/flight_simulator/blob/main/avion_final1.png" alt="Imagen 1" width="45%"/>
  <img src="https://github.com/sepun2004/flight_simulator/blob/main/avion_final2.png" alt="Imagen 2" width="45%"/>
  <img src="https://github.com/sepun2004/flight_simulator/blob/main/avion_final3.png" alt="Imagen 2" width="100%"/>
</p>









