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



