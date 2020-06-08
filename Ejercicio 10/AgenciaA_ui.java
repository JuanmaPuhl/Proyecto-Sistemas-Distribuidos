import java.util.Scanner;

public class AgenciaA_ui extends Thread{

	
	@Override
	public void run()
	{
		while(true) //Tengo que hacer que ejecute siempre...
		{
			System.out.println("Bienvenido a la agencia A.");
			System.out.println("==========================");
			System.out.println("Ingrese la funcion deseada:");
			System.out.println("1. Pedir turno para licencia de matrimonio.");
			System.out.println("2. Obtener informacion de la partida de nacimiento.");
			System.out.println("3. Pedir turno para registrar bebe.");
			System.out.println("4. Patentar el auto.");
			System.out.println("5. Transferir vehiculo.");
			System.out.println("6. Informacion del dominio de vehiculo.");
			System.out.println("0. Salir");

			Scanner scanner = new Scanner(System.in);
			String respuesta = scanner.nextLine();
			System.out.println("El dato ingresado es: "+respuesta);
			/*Inicia la verificacion de la entrada*/
			
			int eleccion;
			try
			{
				eleccion = Integer.parseInt(respuesta);
				if(eleccion>=0 && eleccion<=6)
				{
					//La opcion es correcta
					//Continuo con toda la ejecucion
					String rslt = "";
					switch(eleccion)
					{
						case 0: //Debo cerrar el programa
								System.exit(0);
						case 1: rslt = AgenciaA_main.licenciaMatrimonio();
								System.out.println(rslt);
								break;
						case 2: rslt = AgenciaA_main.partidaNacimiento();
								System.out.println(rslt);
								break;
						case 3: rslt = AgenciaA_main.registrarBebe();
								System.out.println(rslt);
								break;
						default: try {
									System.out.println("Voy a esperar");
									synchronized(AgenciaA_main.recibi){
										if(AgenciaA_main.recibi.equals("")) {
											System.out.println("Me quedo bloqueado");
											AgenciaA_main.recibi.wait();
										}
										System.out.println("Mensaje: "+AgenciaA_main.recibi);
									}
									
									System.out.println("VOLVI PERRAS");
								} catch (InterruptedException e) {
									// TODO Auto-generated catch block
									e.printStackTrace();
								}
							}
				}
				else
				{
					System.out.println("Opcion invalida. Debe ser entre 0 y 6.");
				}
			}catch(NumberFormatException nfe)
			{
				System.out.println("Opcion invalida. Debe ser un numero.");
			}
		}
	}

}
