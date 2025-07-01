import tkinter as tk
from tkinter import ttk, messagebox
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import random
import time

# Simulated sensor data
def read_sensor_data_from_arduino():
    return [
        random.uniform(800, 2000),
        random.uniform(800, 2000),
        random.uniform(800, 2000)
    ]

# Trilateration to get robot position
def trilaterate(p1, r1, p2, r2, p3, r3):
    P1, P2, P3 = np.array(p1), np.array(p2), np.array(p3)
    Ex = (P2 - P1) / np.linalg.norm(P2 - P1)
    i = np.dot(Ex, P3 - P1)
    temp = P3 - P1 - i * Ex
    Ey = temp / np.linalg.norm(temp)
    d = np.linalg.norm(P2 - P1)
    j = np.dot(Ey, P3 - P1)
    x = (r1**2 - r2**2 + d**2) / (2 * d)
    y = (r1**2 - r3**2 + i**2 + j**2 - 2 * i * x) / (2 * j)
    return P1 + x * Ex + y * Ey

# Main GUI class
class RobotGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Cannani Bots (Robot Visualizer)")
        self.root.configure(bg='#1a1a2e')

        self.ref_points = [(0, 0), (3000, 0), (1500, 3000)]
        self.distances = [1000.0, 1800.0, 1500.0]
        self.robot_pos = None
        self.history = []
        self.playback_index = 0

        self.create_widgets()
        self.init_plot()
        self.auto_update()

    # GUI layout
    def create_widgets(self):
        style = ttk.Style()
        style.theme_use('clam')
        style.configure("TButton", font=("Segoe UI", 16, "bold"), padding=10,
                        background="#0f3460", foreground="white")
        style.map("TButton", background=[("active", "#16213e")])
        style.configure("TLabel", background="#1a1a2e", foreground="white", font=("Segoe UI", 12))
        style.configure("TEntry", font=("Segoe UI", 12))

        ttk.Button(self.root, text="Start", command=self.start_system).grid(row=0, column=0, columnspan=2, padx=10, pady=5, sticky="ew")
        ttk.Button(self.root, text="Run", command=self.update_robot_position).grid(row=1, column=0, columnspan=2, padx=10, pady=5, sticky="ew")
        ttk.Button(self.root, text="⏪ Prev", command=self.show_previous).grid(row=2, column=0, padx=10, pady=5)
        ttk.Button(self.root, text="⏩ Next", command=self.show_next).grid(row=2, column=1, padx=10, pady=5)
        ttk.Button(self.root, text="🗑 Clear Logs", command=self.clear_logs).grid(row=3, column=0, padx=10, pady=5)
        ttk.Button(self.root, text="🔄 Reset", command=self.reset_all).grid(row=3, column=1, padx=10, pady=5)
        ttk.Button(self.root, text="📷 Save Path", command=self.save_path_as_image).grid(row=4, column=0, columnspan=2, padx=10, pady=5, sticky="ew")

        self.log_text = tk.Text(self.root, height=12, bg="#0f0f1f", fg="white", font=("Consolas", 10))
        self.log_text.grid(row=6, column=0, columnspan=2, padx=10, pady=5, sticky="ew")
        for level, color in [("INFO", "white"), ("DEBUG", "cyan"), ("WARNING", "orange"), ("ERROR", "red"), ("CRITICAL", "darkred")]:
            self.log_text.tag_config(level, foreground=color)

        self.entries = []
        for i in range(3):
            ttk.Label(self.root, text=f"Distance to Ref Point {i+1} (mm):").grid(row=i+7, column=0, padx=10, pady=5)
            e = ttk.Entry(self.root)
            e.insert(0, str(self.distances[i]))
            e.grid(row=i+7, column=1, padx=10, pady=5)
            self.entries.append(e)

        self.figure, self.ax = plt.subplots(figsize=(6, 6), facecolor='#1a1a2e')
        self.canvas = FigureCanvasTkAgg(self.figure, master=self.root)
        self.canvas.get_tk_widget().grid(row=0, column=2, rowspan=12, padx=10, pady=10)

    # Logger
    def log(self, message, level="INFO"):
        timestamp = time.strftime("%H:%M:%S")
        self.log_text.insert(tk.END, f"[{timestamp}] {level}: {message}\n", level)
        self.log_text.see(tk.END)
        with open("logs.txt", "a") as file:
            file.write(f"[{timestamp}] {level}: {message}\n")

    def clear_logs(self):
        self.log_text.delete(1.0, tk.END)
        with open("logs.txt", "w") as f:
            f.write("")
        self.log("Logs cleared.", "INFO")

    def reset_all(self):
        self.robot_pos = None
        self.history.clear()
        self.playback_index = 0
        for i in range(3):
            self.entries[i].delete(0, tk.END)
            self.entries[i].insert(0, str(self.distances[i]))
        self.init_plot()
        self.log("System reset.", "INFO")

    # Plotting
    def init_plot(self):
        self.ax.clear()
        self.ax.set_title("Cannani Bots (Robot Visualizer)", color='white', fontsize=14)
        self.ax.set_xlim(0, 3000)
        self.ax.set_ylim(0, 3000)
        self.ax.set_facecolor('#0f0f1f')
        self.ax.set_aspect('equal')
        self.ax.grid(True, color='gray', linestyle='--', linewidth=0.5)

        for i, (x, y) in enumerate(self.ref_points):
            self.ax.plot(x, y, 'ro', markersize=10)
            self.ax.text(x + 50, y - 50, f"Ref {i+1}", color='white', fontsize=10)

        self.ax.add_patch(Rectangle((1050, 1050), 900, 900, color='#0074e4', alpha=0.5))
        self.ax.text(1500, 1500, "Future Engineers\n2025 - BZU", color='white', fontsize=10,
                     ha='center', va='center', weight='bold')

        if len(self.history) > 1:
            xs, ys = zip(*self.history)
            self.ax.plot(xs, ys, linestyle='-', color='cyan', linewidth=2, alpha=0.7)

        if self.robot_pos is not None:
            self.ax.plot(self.robot_pos[0], self.robot_pos[1], 'o', markersize=18, color='white')
            self.ax.text(self.robot_pos[0] + 50, self.robot_pos[1] + 50, "Robot", color='white', fontsize=14, fontweight='bold')

        self.ax.tick_params(axis='both', colors='white')
        self.canvas.draw()

    # First reading
    def start_system(self):
        self.log("Starting system...", "INFO")
        data = read_sensor_data_from_arduino()
        if data:
            self.distances = data
            self.log(f"Initial distances: {data}", "DEBUG")
        else:
            self.log("No data received!", "ERROR")
            messagebox.showerror("Error", "Could not read sensor data!")
            return

        for i in range(3):
            self.entries[i].delete(0, tk.END)
            self.entries[i].insert(0, f"{self.distances[i]:.2f}")

        pos = trilaterate(self.ref_points[0], self.distances[0],
                          self.ref_points[1], self.distances[1],
                          self.ref_points[2], self.distances[2])

        if 1050 <= pos[0] <= 1950 and 1050 <= pos[1] <= 1950:
            self.log("Start position in forbidden zone!", "WARNING")
            messagebox.showwarning("Invalid Start", "Start position is inside the forbidden zone!")
            self.robot_pos = None
        else:
            self.robot_pos = pos
            self.history.append(self.robot_pos)
            self.playback_index = len(self.history) - 1
            self.log(f"Start position: {self.robot_pos}", "INFO")

        self.init_plot()

    # Auto updates
    def update_robot_position(self):
        data = read_sensor_data_from_arduino()
        if data:
            r1, r2, r3 = data
            self.robot_pos = trilaterate(self.ref_points[0], r1,
                                         self.ref_points[1], r2,
                                         self.ref_points[2], r3)
            self.history.append(self.robot_pos)
            self.playback_index = len(self.history) - 1
            self.log(f"Position updated: {self.robot_pos}", "DEBUG")
            self.init_plot()
        else:
            self.log("No sensor data received!", "ERROR")
            messagebox.showerror("Error", "No sensor data received!")

    # Playback prev
    def show_previous(self):
        if self.playback_index > 0:
            self.playback_index -= 1
            self.robot_pos = self.history[self.playback_index]
            self.log(f"Playback: Step {self.playback_index + 1}/{len(self.history)}", "INFO")
            self.init_plot()

    # Playback next
    def show_next(self):
        if self.playback_index < len(self.history) - 1:
            self.playback_index += 1
            self.robot_pos = self.history[self.playback_index]
            self.log(f"Playback: Step {self.playback_index + 1}/{len(self.history)}", "INFO")
            self.init_plot()

    # Auto timer
    def auto_update(self):
        self.update_robot_position()
        self.root.after(1000, self.auto_update)

    # Save image
    def save_path_as_image(self):
        fig, ax = plt.subplots(figsize=(6, 6), facecolor='#1a1a2e')
        ax.set_xlim(0, 3000)
        ax.set_ylim(0, 3000)
        ax.set_facecolor('#0f0f1f')
        ax.set_aspect('equal')
        ax.grid(True, color='gray', linestyle='--', linewidth=0.5)

        for i, (x, y) in enumerate(self.ref_points):
            ax.plot(x, y, 'ro', markersize=10)
            ax.text(x + 50, y - 50, f"Ref {i+1}", color='white', fontsize=10)

        ax.add_patch(Rectangle((1050, 1050), 900, 900, color='#0074e4', alpha=0.5))
        ax.text(1500, 1500, "Future Engineers\n2025 - BZU", color='white', fontsize=10,
                ha='center', va='center', weight='bold')

        if len(self.history) > 1:
            xs, ys = zip(*self.history)
            ax.plot(xs, ys, linestyle='-', color='red', linewidth=2, alpha=0.8)

        if self.robot_pos:
            ax.plot(self.robot_pos[0], self.robot_pos[1], 'o', markersize=12, color='white')
            ax.text(self.robot_pos[0] + 50, self.robot_pos[1] + 50, "Robot", color='white', fontsize=12)

        ax.tick_params(axis='both', colors='white')
        ax.set_title("Saved Robot Path", color='white', fontsize=14)
        plt.savefig("robot_path.png", bbox_inches='tight', facecolor=fig.get_facecolor())
        plt.close(fig)
        self.log("Path image saved as 'robot_path.png'", "INFO")

# Run app
if __name__ == "__main__":
    root = tk.Tk()
    app = RobotGUI(root)
    root.mainloop()
