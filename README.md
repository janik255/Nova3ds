# NOVA 3DS — Android-Cloud-Build-Projekt

**Zielgerät:** New Nintendo 3DS / New Nintendo 3DS XL mit Luma3DS  
**Build:** GitHub Actions (devkitPro wird auf dem GitHub-Runner verwendet; nicht auf deinem Android-Handy)  
**Status:** funktionsfähiger Homebrew-Launcher-Prototyp / Entwicklungsgrundlage, **keine fertige Vollversion**.

## Was diese Version tatsächlich enthält

- Startmenü mit NOVA-Branding und Touch-/Tastenbedienung
- Erkennung installierter Titel über die 3DS-AM-Systemdienste (best effort)
- Liste mit Titelname und Titel-ID
- Titel-ID-Browser für installierte SD-Titel (Starten ist noch nicht implementiert)
- einfache Suche über die Tastatur (X)
- Sortierung nach Titelname (Y)
- Favoritenliste lokal als Datei (L)
- Einstellungs-/Infoansicht (R)
- deutsche und englische Grundtexte
- GitHub Actions Workflow zum Bauen einer `.3dsx` und – sofern die verwendete Toolchain es unterstützt – einer `.cia`

## Noch NICHT enthalten

Das ist bewusst keine Behauptung, dass alle gewünschten Funktionen bereits fertig sind. Plugin-Ausführung, Cheat-Engine, Theme-/Mod-/Cheat-Stores mit echten Katalogen, Cover-Downloader, Spielzeit-Tracking, Instaboot und automatische Updates sind in dieser Grundlage **noch nicht implementiert**. Auch das tatsächliche Starten der angezeigten Titel ist in dieser ersten Vorlage noch nicht implementiert. Ein Store benötigt außerdem einen gepflegten, legalen Serverkatalog. NOVA verteilt keine kommerziellen Spiele oder ROMs.

## Android-Anleitung

1. Lade `NOVA_3DS_Android_Build.zip` auf dein Android-Handy.
2. Entpacke die ZIP-Datei mit dem Dateimanager.
3. Öffne im Browser **github.com** und melde dich an oder erstelle einen Account.
4. Erstelle ein neues Repository, z. B. `NOVA-3DS`. Sichtbarkeit kann privat bleiben.
5. Lade den Inhalt dieses Projektordners in das Repository hoch. Wichtig: auch den versteckten Ordner `.github` samt `workflows/build.yml` hochladen. GitHub-Webseite: **Add file → Upload files**.
6. Warte, bis GitHub die Dateien gespeichert hat.
7. Öffne im Repository den Reiter **Actions**. Falls GitHub fragt, aktiviere Workflows.
8. Wähle **Build NOVA 3DS** und tippe **Run workflow**.
9. Nach erfolgreichem Lauf öffnest du den abgeschlossenen Workflow und lädst unter **Artifacts** `NOVA-3DS-build` herunter.
10. Entpacke das Artifact. Diese Version erzeugt eine `NOVA_3DS.3dsx`, keine CIA. Kopiere sie nach `SD:/3ds/NOVA_3DS.3dsx` und starte sie über den Homebrew Launcher.

## SD-Karte

- 3DSX: kopiere `NOVA_3DS.3dsx` nach `SD:/3ds/`.
- Datenordner: kopiere `romfs` nach `SD:/3ds/NOVA_3DS/romfs/` (falls der Build diesen Ordner als separates Paket ausgibt).

**Vor Änderungen:** Sichere deine SD-Karte. Verwende nur Homebrew und Inhalte, für die du Nutzungsrechte hast.

## Steuerung (Prototyp)

- Steuerkreuz / Circle Pad: Auswahl
- A: reserviert (Startfunktion folgt)
- B: zurück
- X: Suche (Systemtastatur)
- Y: alphabetisch sortieren
- L: Favorit umschalten
- R: Info

## Fehlerbehebung

- **Actions-Tab fehlt:** Repository-Seite neu laden; ggf. unter Settings → Actions → General erlauben.
- **Build schlägt fehl:** Öffne den fehlgeschlagenen Lauf und kopiere die rote Fehlermeldung. Nicht einfach eine nicht erzeugte CIA auf die SD-Karte kopieren.
- **CIA fehlt, 3DSX vorhanden:** Der CIA-Packaging-Schritt wurde übersprungen oder ist fehlgeschlagen. Das 3DSX ist trotzdem ein Homebrew-Build, aber nicht per FBI installierbar.
- **Titel werden nicht angezeigt:** AM-Zugriff kann je nach Umgebung/Berechtigungen scheitern; starte mit Luma3DS und prüfe, ob die App als Homebrew korrekt ausgeführt wird.

## Technische Hinweise

Das Projekt verwendet devkitPro/devkitARM und libctru im Build-Container. Der GitHub-Runner baut die Anwendung; auf dem Android-Gerät muss keine Toolchain installiert werden. Eine erfolgreiche Kompilierung ist keine Garantie für fehlerfreie Laufzeit auf jeder Firmware-Version.
