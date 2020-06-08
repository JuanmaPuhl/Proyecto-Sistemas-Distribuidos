import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.PushbackInputStream;
import java.net.Socket;

public class AgenciaA_main extends Thread{
	public final static String HOST = "127.0.0.1";
	public final static int PUERTO = 5000;
	public static DataInputStream in;
	public static DataOutputStream out;
	public static Socket sc;
	public static PushbackInputStream pbis;
	public final static int PEDIRFUNCION = 0;
	public final static int DARFUNCION = 1;
	public static AgenciaA_ui hilo1;
	public static AgenciaA_msgReceiver hilo2 ;
	public static String recibi = "";
	public static String licenciaMatrimonio()
	{
		return "Registrando licencia matrimonio.";
	}
	
	public static String partidaNacimiento()
	{
		return "Informacion partida nacimiento.";
	}
	
	public static String registrarBebe()
	{
		return "Registrando bebe.";
	}
	
	
	/**Funcion privada utilizada para pedir datos que no estan en el cliente.*/
	public static void pedirASv(int fn)
	{
		try
		{
			
			//Envio el encabezado correspondiente primero
			out.writeInt(PEDIRFUNCION);
			out.writeUTF(""+fn); //Envio la funcion a pedir
			pbis = new PushbackInputStream(in);
			int encabezado = pbis.read();
			if(encabezado == 1) {
				System.out.println("LLEGO UN UNO");
				pbis.unread(encabezado);
				int encabezado2 = pbis.read();
				if(encabezado2 == 1) {
					System.out.println("LLEGO UN UNO DE NUEVO, FUNCIONA");
					System.out.println("Ahora leo el stream pelado "+in.readInt());
				}
			}
			else
				System.out.println("Lo que llego fue: "+encabezado);
				System.out.println("Ahora leo el stream pelado "+in.readInt());
			
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		
	}
	
	public static void main(String[] args)
    {

    	/*Imprimo el menu*/
		
		try {
			sc = new Socket(HOST,PUERTO);
			in = new DataInputStream(sc.getInputStream());
			out = new DataOutputStream(sc.getOutputStream());
			hilo1 = new AgenciaA_ui();
			hilo2 = new AgenciaA_msgReceiver();
			hilo1.start();
			hilo2.start();
			
		} catch (IOException e)
		{
			e.printStackTrace();
		}
		

		
		
		
    	
    }
}
