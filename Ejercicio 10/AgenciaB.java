import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

public class AgenciaB {
	public static void main(String[] args)
    {
    	final String HOST = "127.0.0.1";
    	final int PUERTO = 5000;
		DataInputStream in;
		DataOutputStream out;
    	/*Imprimo el menu*/
		
		while(true) //Tengo que hacer que ejecute siempre...
		{
			System.out.println("Bienvenido a la agencia B.");
			System.out.println("==========================");
			System.out.println("Ingrese la funcion deseada:");
			System.out.println("1. Pedir turno para licencia de matrimonio.");
			System.out.println("2. Obtener informacion de la partida de nacimiento.");
			System.out.println("3. Pedir turno para registrar bebe.");
			System.out.println("4. Patentar el auto.");
			System.out.println("5. Transferir vehiculo.");
			System.out.println("6. Informacion del dominio de vehiculo.");
			System.out.println("0. Salir");

			
			try {
				Socket sc = new Socket(HOST,PUERTO);
				in = new DataInputStream(sc.getInputStream());
				out = new DataOutputStream(sc.getOutputStream());
				
				out.writeUTF("Hola server");
				String mensaje = in.readUTF();
				System.out.println(mensaje);
				sc.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
    }
		
}
