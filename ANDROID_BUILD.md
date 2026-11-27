# Android-Cloud-Build

1. ZIP-Datei auf dem Handy herunterladen und entpacken.
2. Auf github.com anmelden und ein neues Repository erstellen.
3. Im Repository **Add file → Upload files** wählen.
4. Alle Dateien aus dem entpackten Ordner auswählen. Versteckte Dateien/Ordner anzeigen lassen und `.github/workflows/build.yml` unbedingt mit hochladen.
5. Änderungen speichern (Commit).
6. Reiter **Actions** öffnen, den Workflow **Build NOVA 3DS** auswählen und falls nötig **Enable workflow** antippen.
7. **Run workflow** ausführen oder den automatisch gestarteten Build öffnen.
8. Nach grünem Haken den Lauf öffnen und unter **Artifacts** `NOVA-3DS-build` herunterladen.
9. ZIP des Artifacts entpacken. `NOVA_3DS.3dsx` nach `SD:/3ds/` kopieren.
10. SD-Karte sicher auswerfen, in den 3DS stecken und die Datei über den Homebrew Launcher starten.

Diese Vorlage erzeugt keine CIA. Eine CIA ist ein separates Installationspaket mit Metadaten und gültiger Paketierung. FBI installiert nur eine bereits erzeugte CIA; es wandelt keine 3DSX in CIA um. Diese Vorlage ist außerdem ein früher Prototyp: Sie zeigt installierte SD-Titel-IDs an, startet sie aber noch nicht.
