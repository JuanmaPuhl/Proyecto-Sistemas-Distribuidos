
// SimpleClient.java: A simple client program.
import java.net.*;
import java.util.Scanner;
import java.io.*;

public class SimpleClient {
	public static void main(String args[]) throws IOException {

		if (args.length != 2) {
			System.err.println("Usage: java client <host name> <port number>");
			System.exit(1);
		}

		String hostName = args[0];
		int portNumber = Integer.parseInt(args[1]);

		// Open your connection to a server
		Socket s1 = new Socket(hostName, portNumber);
		// Get an input file handle from the socket and read the input
		Scanner scanln = new Scanner(System.in);

		System.out.println("Mini Operaciones");
		System.out.println("-----------------");
		texto();
		int opcion = scanln.nextInt();
		while (!((opcion >= 1) && (opcion <= 6))) {
			System.out.println("Elija una opcion valida.");
			opcion = scanln.nextInt();
			texto();
		}
		int[] nums = new int[4];
		int cant_op = 0;
		if (opcion == 1 || opcion == 2) {
			cant_op = 1;
		}
		if (opcion == 3 || opcion == 4) {
			cant_op = 4;
		}
		if (opcion == 5 || opcion == 6) {
			cant_op = 2;
		}

		for (int i = 0; i < cant_op; i++) {
			System.out.println("Ingrese el numero " + (i + 1) + ":");
			nums[i] = scanln.nextInt();
		}

		OutputStream s1out = s1.getOutputStream();
		DataOutputStream dos = new DataOutputStream(s1out);
		InputStream s1In = s1.getInputStream();
		DataInputStream dis = new DataInputStream(s1In);
		int resultado = 0;

		long startTime = System.nanoTime();
		long elapsedTime = 0;
		dos.writeInt(opcion);
		for (int i = 0; i < cant_op; i++) {
			dos.writeInt(nums[i]);
		}

		if (opcion == 1) {
			resultado = dis.readInt();
			elapsedTime = System.nanoTime() - startTime;
			System.out.println("Binario: " + resultado);
		}
		if (opcion == 2) {
			String hexa = new String(dis.readUTF());
			elapsedTime = System.nanoTime() - startTime;
			System.out.println("Hexadecimal: " + hexa);
		}
		if (opcion == 3) {
			resultado = dis.readInt();
			elapsedTime = System.nanoTime() - startTime;
			System.out.println("Suma: " + resultado);
		}
		if (opcion == 4) {
			resultado = dis.readInt();
			elapsedTime = System.nanoTime() - startTime;
			System.out.println("Resta: " + resultado);
		}
		if (opcion == 5) {
			resultado = dis.readInt();
			elapsedTime = System.nanoTime() - startTime;
			System.out.println("Multiplicacion: " + resultado);
		}
		if (opcion == 6) {
			resultado = dis.readInt();
			int resto = dis.readInt();
			elapsedTime = System.nanoTime() - startTime;
			System.out.println("Division: Cociente = " + resultado + ", resto = " + resto);
		}
		
		System.out.println("Tiempo en microsegundos: "+ elapsedTime/1000);
		
		// When done, just close the connection and exit
		dis.close();
		s1In.close();
		s1.close();
		scanln.close();
	}

	private static void texto() {
		System.out.println("Seleccione una opcion:");
		System.out.println("1) Conversion de numero decimal a binario.");
		System.out.println("2) Conversion de numero binario a hexadecimal.");
		System.out.println("3) Suma de hasta cuatro operandos.");
		System.out.println("4) Resta de hasta cuatro operandos.");
		System.out.println("5) Multiplicacion de dos operandos.");
		System.out.println("6) Division de hasta dos operandos.");
	}
}
