/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.*;
import java.util.*;
import java.net.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import static java.lang.System.out;

/**
 *
 * @author Daniel Tse
 */
public class Client extends JFrame implements ActionListener {
    Socket client;
    BufferedReader reader;
    PrintWriter writer;
    JTextArea messageArea;
    JTextField messageInput;
    JButton send;

    public Client(String user, String serverNum, String servername) throws Exception {
        super("User: " + user);
        int num = Integer.parseInt(serverNum);
        client = new Socket(servername, num);
        reader = new BufferedReader(new InputStreamReader(client.getInputStream()));
        writer = new PrintWriter(client.getOutputStream(), true);
        writer.println(user);
        createChatWindow();
        new MessagesThread().start();
    }

    public void createChatWindow() {

        // text field
        messageArea = new JTextArea();
        messageArea.setRows(25);
        messageArea.setColumns(30);
        messageArea.setEditable(false);

        // input field
        messageInput = new JTextField(30);
        // button
        send = new JButton("Send");
        // scrollbar
        JScrollPane scrollBar = new JScrollPane(messageArea, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,
                JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        add(scrollBar, "Center");

        JPanel inputPanel = new JPanel(new FlowLayout());
        inputPanel.add(messageInput);
        inputPanel.add(send);
        add(inputPanel, "South");
        send.addActionListener(this);
        ;
        setVisible(true);
        pack();
    }

    public void actionPerformed(ActionEvent evt) {
        writer.println(messageInput.getText());
    }

    class MessagesThread extends Thread {
        public void run() {
            String line;
            try {
                while (true) {
                    line = reader.readLine();
                    messageArea.append(line + "\n");
                }
            } catch (Exception ex) {
            }
        }
    }

    public static void main(String[] args) {
        JTextField name = new JTextField(20);
        JTextField serverNum = new JTextField("5190", 5); // jtextfield given default server

        JPanel inputPanel = new JPanel();
        inputPanel.add(new JLabel("Username: "));
        inputPanel.add(name);
        inputPanel.add(Box.createHorizontalStrut(10));
        inputPanel.add(new JLabel("Server: "));
        inputPanel.add(serverNum);

        int result = JOptionPane.showConfirmDialog(null, inputPanel, "Enter Username and Server",
                JOptionPane.OK_CANCEL_OPTION);

        if (result == JOptionPane.OK_OPTION) {
            String servername = "localhost";
            try {
                new Client(name.getText(), serverNum.getText(), servername);
            } catch (Exception ex) {
                out.println("Error --> " + ex.getMessage());
            }
        }
    }
}