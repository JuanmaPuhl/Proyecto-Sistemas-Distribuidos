import java.net.*;
import java.io.*;

public class Server
{
	private final static int PEDIRFUNCION = 0;
	private final static int DARFUNCION = 1;
  
    public static void main(String[] args)
    {
        ServerSocket servidor = null;
        Socket sc = null;
        final int PUERTO = 5000;
        
        try
        {
			servidor = new ServerSocket(PUERTO);
			System.out.println("Servidor iniciado");
			DataInputStream in;
			DataOutputStream out;
			while(true)
			{
				sc = servidor.accept();
				System.out.println("Cliente conectado");
				in = new DataInputStream(sc.getInputStream());
				out = new DataOutputStream(sc.getOutputStream());
				int encabezado = in.readInt();
				System.out.println(encabezado);
				if(encabezado == PEDIRFUNCION)
				{
					System.out.println("ENTRE AQUI COMPADRE");
					String funcion = in.readUTF();
					System.out.println(funcion);
					out.writeByte('1');
				}
				sc.close();
				System.out.println("Cliente desconectado");
			}
			
		} catch (IOException e)
        {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
    }
}
