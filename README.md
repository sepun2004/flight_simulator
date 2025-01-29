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

## Controles

| Tecla | Acción |
|-------|--------|
| ⬆️ (Up) | Elevar el avión |
| ⬇️ (Down) | Descender el avión |
| ⬅️ (Left) | Girar a la izquierda |
| ➡️ (Right) | Girar a la derecha |
| W | Aumentar velocidad |
| S | Disminuir velocidad |
