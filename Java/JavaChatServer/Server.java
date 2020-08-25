/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.*;
import java.util.*;
import java.net.*;
import static java.lang.System.out;

/**
 *
 * @author Daniel Tse
 */
public class Server {
	Vector<String> users = new Vector<String>();
	Vector<ClientProcess> clients = new Vector<ClientProcess>();

	class ClientProcess extends Thread {
		BufferedReader in;
		PrintWriter out;
		String name;

		public ClientProcess(Socket client) throws Exception {
			in = new BufferedReader(new InputStreamReader(client.getInputStream()));
			out = new PrintWriter(client.getOutputStream(), true);
			name = in.readLine();
			users.add(name);
			start();
		}

		public void run() {
			String line;
			try {
				while (true) {
					line = in.readLine();
					if (line.equals("end")) {
						clients.remove(this);
						users.remove(name);
						break;
					}
					sendAll(name, line);
				}
			} catch (Exception ex) {
				System.out.println(ex.getMessage());
			}
		}

		public void send(String user, String msg) {
			out.println(user + ": " + msg);
		}
	}

	public void process() throws Exception {
		ServerSocket server = new ServerSocket(5190, 10);
		while (true) {
			Socket client = server.accept();
			clients.add(new ClientProcess(client));
		}
	}

	// send message to all users
	public void sendAll(String user, String message) {
		for (ClientProcess client : clients) {
			client.send(user, message);
		}
	}

	public static void main(String[] args) throws Exception {
		new Server().process();
	}
}