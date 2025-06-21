import tkinter as tk
from tkinter import ttk, messagebox
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import random


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

    position = P1 + x * Ex + y * Ey
    return position


def read_distance_from_sensor(index):
    return random.uniform(500, 2500)


class RobotGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Cannani Bots (Robot Visualizer)")
        self.root.configure(bg='#1a1a2e')

        self.ref_points = [(0, 0), (3000, 0), (1500, 3000)]
        self.distances = [1000.0, 1800.0, 1500.0]
        self.robot_pos = None

        self.create_widgets()
        self.init_plot()

    def create_widgets(self):
        style = ttk.Style()
        style.theme_use('clam')
        style.configure("TButton", font=("Segoe UI", 16, "bold"),
                        padding=10, background="#0f3460", foreground="white")
        style.map("TButton", background=[("active", "#16213e")])
        style.configure("TLabel", background="#1a1a2e", foreground="white", font=("Segoe UI", 12))
        style.configure("TEntry", font=("Segoe UI", 12))

        ttk.Button(self.root, text="Start", command=self.start_system).grid(row=0, column=0, padx=10, pady=15, columnspan=2, sticky="ew")
        ttk.Button(self.root, text="Run", command=self.update_robot_position).grid(row=1, column=0, padx=10, pady=15, columnspan=2, sticky="ew")

        self.entries = []
        for i in range(3):
            ttk.Label(self.root, text=f"Distance to Ref Point {i+1} (mm):").grid(row=i+2, column=0, padx=10, pady=5)
            e = ttk.Entry(self.root)
            e.insert(0, str(self.distances[i]))
            e.grid(row=i+2, column=1, padx=10, pady=5)
            self.entries.append(e)

        self.figure, self.ax = plt.subplots(figsize=(6, 6), facecolor='#1a1a2e')
        self.canvas = FigureCanvasTkAgg(self.figure, master=self.root)
        self.canvas.get_tk_widget().grid(row=0, column=2, rowspan=6, padx=10, pady=10)

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

        
        if self.robot_pos:
            self.ax.plot(self.robot_pos[0], self.robot_pos[1], 'o', markersize=18, color='white')
            self.ax.text(self.robot_pos[0] + 50, self.robot_pos[1] + 50, "Robot", color='white', fontsize=14, fontweight='bold')

        
        self.ax.tick_params(axis='both', colors='white')

        self.canvas.draw()

    def start_system(self):
        self.distances = [read_distance_from_sensor(i) for i in range(3)]
        for i in range(3):
            self.entries[i].delete(0, tk.END)
            self.entries[i].insert(0, f"{self.distances[i]:.2f}")

        pos = trilaterate(self.ref_points[0], self.distances[0],
                          self.ref_points[1], self.distances[1],
                          self.ref_points[2], self.distances[2])

        if 1050 <= pos[0] <= 1950 and 1050 <= pos[1] <= 1950:
            messagebox.showwarning("Invalid Start", "Start position is inside the forbidden (blue) zone!")
            self.robot_pos = None
        else:
            self.robot_pos = pos

        self.init_plot()

    def update_robot_position(self): # Running and taking inputs 
        try:
            r1 = float(self.entries[0].get())
            r2 = float(self.entries[1].get())
            r3 = float(self.entries[2].get())
            self.robot_pos = trilaterate(self.ref_points[0], r1,
                                         self.ref_points[1], r2,
                                         self.ref_points[2], r3)
            self.init_plot()
        except Exception as e:
            messagebox.showerror("Error", f"Failed to update robot position:\n{e}")


if __name__ == "__main__":
    root = tk.Tk()
    app = RobotGUI(root)
    root.mainloop()
