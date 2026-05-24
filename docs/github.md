# Git y GitHub

## Repositorio local

Este proyecto se puede versionar con:

```powershell
git init
git add .
git commit -m "Crea base de cronometro ESP32 con pantalla TFT"
```

## Crear repositorio en GitHub

En esta computadora no esta disponible `gh` al momento de crear la base. Tienes dos opciones.

### Opcion A: desde la web

1. Entra a GitHub.
2. Crea un repositorio nuevo llamado `cronometro-pantalla-esp32`.
3. No agregues README, `.gitignore` ni licencia desde GitHub.
4. Copia la URL del repositorio.
5. Ejecuta:

```powershell
git remote add origin https://github.com/TU_USUARIO/cronometro-pantalla-esp32.git
git branch -M main
git push -u origin main
```

### Opcion B: con GitHub CLI

Instala GitHub CLI, inicia sesion con `gh auth login` y luego ejecuta:

```powershell
gh repo create cronometro-pantalla-esp32 --private --source . --remote origin --push
```

Puedes cambiar `--private` por `--public` si quieres que el proyecto sea publico.
