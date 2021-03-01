package com.company;

import java.awt.*;
import javax.swing.*;
import javax.imageio.ImageIO;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.IOException;

public class Transforms2D extends JPanel {
    private class Display extends JPanel {
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            Graphics2D g2 = (Graphics2D) g;
            g2.translate(300, 300);  // Moves (0,0) to the center of the display.
            int whichTransform = transformSelect.getSelectedIndex();
            AffineTransform tx = new AffineTransform();

            switch (whichTransform) {
                case 1:
                    g2.scale(.5, .5);
                    break;

                case 2:
                    g2.rotate(Math.toRadians(45), 0, 0);
                    break;

                case 7:
                    tx.scale(.5, 1.25);
                    tx.rotate(Math.toRadians(180), 0, 0);
                    g2.transform(tx);
                    break;

                case 4:
                    tx.scale(1.25, 1.25);
                    tx.shear(.25, 0);
                    g2.transform(tx);
                    break;

                case 5:
                    tx.scale(1.25, .5);
                    tx.translate(0, -450);
                    g2.transform(tx);
                    break;

                case 6:
                    tx.shear(0, -.25);
                    tx.rotate(Math.toRadians(90), 0, 0);
                    tx.scale(1.25, 1.25);
                    g2.transform(tx);
                    break;

                case 3:
                    tx.rotate(Math.toRadians(180));
                    tx.scale(-.5, 1.25);
                    g2.transform(tx);
                    break;
                case 8:
                    tx.translate(-50, 100);
                    tx.rotate(Math.toRadians(30));
                    tx.scale(1.0625, .5);
                    g2.transform(tx);
                    break;

                case 9:
                    tx.rotate(Math.toRadians(180));
                    tx.scale(1.125, 1.125);
                    tx.translate(-115, 0);
                    tx.shear(0, .25);
                    g2.transform(tx);
                    break;

                case 0: // intentional fallthrough
                default:
            }

            alterPolygon(m_poly, 8, 0, 150);

            g2.drawPolygon(m_poly);
        }
    }

    private Display display;
    private BufferedImage pic;
    private Polygon m_poly;
    private JComboBox<String> transformSelect;

    public Transforms2D() throws IOException {
        pic = ImageIO.read(getClass().getClassLoader().getResource("shuttle.jpg"));
        m_poly = new Polygon();
        display = new Display();
        display.setBackground(Color.YELLOW);
        display.setPreferredSize(new Dimension(600, 600));
        transformSelect = new JComboBox<String>();
        transformSelect.addItem("None");
        for (int i = 1; i < 10; i++) {
            transformSelect.addItem("No. " + i);
        }
        transformSelect.addActionListener(e -> display.repaint());
        setLayout(new BorderLayout(3, 3));
        setBackground(Color.GRAY);
        setBorder(BorderFactory.createLineBorder(Color.GRAY, 10));
        JPanel top = new JPanel();
        top.setLayout(new FlowLayout(FlowLayout.CENTER));
        top.setBorder(BorderFactory.createEmptyBorder(4, 4, 4, 4));
        top.add(new JLabel("Transform: "));
        top.add(transformSelect);
        add(display, BorderLayout.CENTER);
        add(top, BorderLayout.NORTH);
    }

    private static void alterPolygon(Polygon p, int n, int x, int r) {
        for (int i = 0; i < n; i++)
            p.addPoint((int) (x + r * Math.cos(i * 2 * Math.PI / n)),
                    (int) (x + r * Math.sin(i * 2 * Math.PI / n)));
    }


    public static void main(String[] args) throws IOException {
        JFrame window = new JFrame("2D Transforms");
        window.setContentPane(new Transforms2D());
        window.pack();
        window.setResizable(false);
        window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
        window.setLocation((screen.width - window.getWidth()) / 2, (screen.height - window.getHeight()) / 2);
        window.setVisible(true);
    }

}