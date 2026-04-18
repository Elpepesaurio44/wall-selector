# 🛰️ ZeroSignal | wall-selector

[![AUR version](https://img.shields.io/aur/version/wall-selector?color=1793d1&logo=arch-linux)](https://aur.archlinux.org/packages/wall-selector/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/Made%20with-C%2B%2B-00599C?logo=c%2B%2B)](https://isocpp.org/)

**wall-selector** es una herramienta minimalista y rápida escrita en C++ para gestionar fondos de pantalla en entornos **Wayland**. Fue creada específicamente para funcionar en hardware con recursos limitados (como procesadores Intel Celeron), ofreciendo una interfaz gráfica ligera pero potente.

---

## 🚀 Características Principales

* **⚡ Ultra Ligero:** Consumo mínimo de RAM y CPU.
* **🎨 Integración con Pywal:** Genera esquemas de colores para tu terminal dinámicamente.
* **🖼️ Selección Visual:** Previsualiza y cambia wallpapers con un solo clic.
* **🎲 Botón Aleatorio:** Para cuando no sabes qué fondo elegir.
* **🐧 Native Arch:** Disponible en el AUR para una instalación limpia.

## 📦 Instalación

### Método AUR (Arch Linux)
Si usas Arch o derivados, simplemente instala con tu helper favorito:

```bash
yay -S wall-selector
````

### Compilación desde el código fuente

Si deseas compilarlo manualmente, necesitas las dependencias `gtkmm3`, `swaybg` y `python-pywal`:

```bash
git clone https://github.com/Elpepesaurio44/wall-selector.git
cd wall-selector
make
sudo make install
```

## 🛠️ Cómo funciona

1.  El programa escanea tu carpeta de imágenes configurada.
2.  Al seleccionar una imagen, se ejecuta `swaybg` para cambiar el fondo en Wayland.
3.  Simultáneamente, llama a `wal -i` para actualizar los colores de tu sistema.
4.  Para cambiar la ruta de los wallpaper presiona control+s para buscar la carpeta

-----

## 🎬 Demostración en Video

¡Mira a **ZeroSignal wall-selector** en acción\!




https://github.com/user-attachments/assets/26cfff49-aea9-497b-b38c-cb038e5bb7d9




-----

## 🤝 Contribuir

¡Las sugerencias son bienvenidas\! Si tienes una idea para mejorar el rendimiento o añadir una función:

1.  Haz un Fork del proyecto.
2.  Crea una rama con tu mejora (`git checkout -b feature/mejora`).
3.  Haz un commit (`git commit -m 'Añadida nueva función'`).
4.  Haz un Push (`git push origin feature/mejora`).
5.  Abre un Pull Request.

-----

**Desarrollado por [Elpepesaurio44](https://github.com/Elpepesaurio44) **

