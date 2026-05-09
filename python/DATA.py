# ─────────────────────────────────────────────────────────────────────────────
# DATA.py — Librairie d'enregistrement CSV côté MPU
#
# RÉCUPÉRATION DES FICHIERS APRÈS SESSION :
#   Les fichiers CSV sont stockés dans le conteneur Docker de l'application.
#   Pour les copier vers le système hôte après chaque session, exécuter
#   depuis le shell Linux de l'UNO Q :
#
#   docker cp <nom_app>-main-1:/app/python/logs /home/arduino/logs
#
#   Remplacer <nom_app> par le nom de votre projet dans l'App Lab.
#   Exemple :
#   docker cp magnetometer-library-data-main-1:/app/python/logs /home/arduino/logs
#
#   Note : cette limitation est due à l'isolation Docker du conteneur MPU.
#   Une issue a été ouverte sur github.com/arduino/arduino-app-cli
#   pour demander un volume monté persistant.
# ─────────────────────────────────────────────────────────────────────────────

import csv
import os


class DATA:
    """
    Gère la création et l'écriture des fichiers CSV de session.

    Usage :
        db = DATA(["timestamp_ms", "heading_deg"])
        db.write({"timestamp_ms": 1234, "heading_deg": 161})
    """

    def __init__(self, headers: list, log_dir: str = None):
        """
        Initialise la librairie DATA.

        @param headers  : liste des colonnes CSV, dans l'ordre d'écriture
        @param log_dir  : dossier de stockage (défaut : logs/ à côté de DATA.py)
        """
        if log_dir is None:
            log_dir = os.path.join(os.path.dirname(os.path.dirname(__file__)), "logs")

        self._headers = headers
        self._log_file = self._initialize_log_file(log_dir)

    # ─────────────────────────────────────────────────────────────────────────
    # Méthodes publiques
    # ─────────────────────────────────────────────────────────────────────────

    def write(self, row: dict):
        """
        Écrit une ligne dans le fichier CSV de la session courante.

        @param row : dictionnaire {nom_colonne: valeur}
                     Les clés doivent correspondre aux headers définis à l'init.
        """
        try:
            with open(self._log_file, "a", newline="") as f:
                writer = csv.DictWriter(f, fieldnames=self._headers)
                writer.writerow(row)
        except Exception as e:
            print(f"[DATA] ERREUR écriture : {e}", flush=True)

    # ─────────────────────────────────────────────────────────────────────────
    # Méthodes privées
    # ─────────────────────────────────────────────────────────────────────────

    def _get_next_session_number(self, log_dir: str) -> int:
        """Retourne le prochain numéro de session disponible."""
        if not os.path.exists(log_dir):
            return 1
        max_num = 0
        for filename in os.listdir(log_dir):
            if filename.startswith("session_") and filename.endswith(".csv"):
                try:
                    num = int(filename[8:-4])
                    if num > max_num:
                        max_num = num
                except ValueError:
                    pass
        return max_num + 1

    def _snapshot_config(self, log_file: str, config_path: str):
        """
        Lit CONFIG.h et l'écrit en commentaire en tête du fichier CSV.
        Si CONFIG.h est introuvable, écrit un avertissement et continue.
        """
        try:
            with open(config_path, "r") as f:
                config_content = f.read()
            with open(log_file, "a", newline="") as f:
                for line in config_content.splitlines():
                    f.write(f"# {line}\n")
            print(f"[DATA] CONFIG.h snapshotté dans {log_file}", flush=True)
        except FileNotFoundError:
            print(f"[DATA] AVERTISSEMENT : CONFIG.h introuvable à {config_path}", flush=True)

    def _initialize_log_file(self, log_dir: str) -> str:
        """Crée le dossier et le fichier CSV de session avec son en-tête."""
        os.makedirs(log_dir, exist_ok=True)
        session_num = self._get_next_session_number(log_dir)
        log_file = os.path.join(log_dir, f"session_{session_num:04d}.csv")
    
        # Chemin de CONFIG.h : /app/sketch/CONFIG.h
        config_path = os.path.join(
            os.path.dirname(os.path.dirname(__file__)), "sketch", "CONFIG.h"
        )
    
        with open(log_file, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=self._headers)
            writer.writeheader()
    
        # Snapshot de CONFIG.h en tête de fichier
        # Note : les lignes préfixées par # sont ignorées par csv.DictReader
        self._snapshot_config(log_file, config_path)
    
        print(f"[DATA] Session {session_num:04d} → {log_file}", flush=True)
        return log_file