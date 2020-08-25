/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package hw5;

import java.io.*;

import javax.swing.event.*;
import java.awt.BorderLayout;
import java.net.*;
import java.awt.Graphics2D;
import java.awt.Dimension;

import java.text.*;
import java.awt.Graphics;
import java.awt.geom.*;
import javax.swing.*;
/**
 *
 * @author Daniel Tse
 */

public class analogClock implements Runnable {
    static Thread t1, t2;
    static int[] globalTime = new int[3];

    private final long interval;
    private final boolean calibrate;

    public analogClock(long interval, boolean calibrate) {

        this.interval = interval;
        this.calibrate = calibrate;
    }

    @Override
    public void run() {
        while (true) {
            if (calibrate) {
                globalTime = getTime();
                //System.out.println("cal: " + globalTime[0] + ":" + globalTime[1] + ":" + globalTime[2]);
            } else { // update
                iterateTime();
                //System.out.println("upd: " + globalTime[0] + ":" + globalTime[1] + ":" + globalTime[2]);
            }

            try {
                Thread.sleep(interval);
            } catch (InterruptedException e) {
                return;
            }
        }
    }

    static void begin() {
        t1 = new Thread(new analogClock(1000, false));
        t2 = new Thread(new analogClock(60000, true));

        t1.start();
        t2.start();
    }

    public void iterateTime() {
        // seconds
        globalTime[2] += 1;
        if (globalTime[2] >= 60) {
            // minutes
            globalTime[1] += 1;
            globalTime[2] = 0;

            if (globalTime[1] >= 60) {
                // hours
                globalTime[0] += 1;
                globalTime[1] = 0;

                if (globalTime[0] >= 24) {
                    globalTime[0] = 0;
                }
            }
        }
    }

    public static int[] getTime() {
        Socket socket = null;

        int intTime[] = new int[3];

        try {
            socket = new Socket("time.nist.gov", 13);
            socket.setSoTimeout(15000);
            InputStream in = socket.getInputStream();

            StringBuilder time = new StringBuilder();
            InputStreamReader reader = new InputStreamReader(in, "ASCII");

            for (int c = reader.read(); c != -1; c = reader.read()) {
                time.append((char) c);
            }

            String strtime = time.toString();
            String[] arrOfStr = strtime.split(" ", 4);

            String timeSegment = arrOfStr[2]; // element 2 is hh:mm:ss
            String[] hms = timeSegment.split(":", 3);

            for (int i = 0; i < intTime.length; i++) {
                intTime[i] = Integer.parseInt(hms[i]);
            }

        } catch (IOException ex) {
            System.err.println(ex);
        } finally {
            if (socket != null) {
                try {
                    socket.close();
                } catch (IOException ex) {
                }
            }
        }
        return intTime;
    }

    public static int[] broadcastTime() {
        return globalTime;
    }

    public static void main(String args[]) {
        globalTime = getTime();
        begin();

        ClockFrame frame = new ClockFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        while (true) {
            frame.updateClock();
        }
    }
}

class ClockFrame extends JFrame {
    private Panel clock;
    int[] globalTime = new int[] { 0, 0, 0 };

    public ClockFrame() {
        setTitle("Analog Clock");
        clock = new Panel();
        add(clock, BorderLayout.CENTER);
        pack();
    }

    public void updateClock() {

        globalTime = analogClock.broadcastTime();
        //System.out.println("Receiving: " + globalTime[0] + ":" + globalTime[1] + ":" + globalTime[2]);
        int hours = globalTime[0];
        int minutes = globalTime[1];
        int seconds = globalTime[2];
        clock.updateTime(hours, minutes, seconds);

    }
}

class Panel extends JPanel {
    private int radius = 200;
    private double lengthHour = 0.6 * radius;
    private double lengthMinute = 0.8 * radius;
    private double lengthSecond = 0.9 * radius;

    private double hours = 0;
    private double minutes = 0;
    private double seconds = 0;

    public Panel() {
        setPreferredSize(new Dimension(2 * radius, 2 * radius));
    }

    public void updateTime(int h, int m, int s) {
        hours = h;
        minutes = m;
        seconds = s;
        repaint();
    }

    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2 = (Graphics2D) g;
        Ellipse2D circle = new Ellipse2D.Double(0, 0, 2 * radius, 2 * radius);
        g2.draw(circle);

        double hourAngle = Math.toRadians(90 - 360 * hours / 12);
        drawHand(g2, hourAngle, lengthHour);

        double minuteAngle = Math.toRadians(90 - 360 * minutes / 60);
        drawHand(g2, minuteAngle, lengthMinute);

        double secondAngle = Math.toRadians(90 - 360 * seconds / 60);
        drawHand(g2, secondAngle, lengthSecond);
    }

    public void drawHand(Graphics2D g2, double angle, double handLength) {
        Point2D center = new Point2D.Double(radius, radius);
        Point2D end = new Point2D.Double(radius + handLength * Math.cos(angle), radius - handLength * Math.sin(angle));
        g2.draw(new Line2D.Double(center, end));
    }
}
