import java.net.*;
import java.io.*;

public class SimpleServerThread extends Thread {
	private Socket s1 = null;

	public SimpleServerThread(Socket socket) {
		super("SimpleServerThread");
		this.s1 = socket;
	}

	public void run() {

		try {
			// Get a communication stream associated with the socket
			InputStream s1In = s1.getInputStream();
			DataInputStream dis = new DataInputStream(s1In);

			int[] nums = new int[4];
			int cant_op = 0;
			int opcion = (dis.readInt());
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
				nums[i] = (dis.readInt());
			}
			OutputStream s1out = s1.getOutputStream();
			DataOutputStream dos = new DataOutputStream(s1out);
			int ans = 0;
			if (opcion == 1) {
				int rem, temp = 1;
				int resultado = 0;
				while (nums[0] > 0) {
					rem = nums[0] % 2;
					nums[0] = nums[0] / 2;
					resultado = resultado + rem * temp;
					temp = temp * 10;
				}
				ans = resultado;
				dos.writeInt(ans);
			}
			if (opcion == 2) {
				String bin = Integer.toString(nums[0]);
				int num = Integer.parseInt(bin, 2);
				String hexa = Integer.toHexString(num);
				dos.writeUTF(hexa);
			}
			if (opcion == 3) {
				ans = nums[0] + nums[1] + nums[2] + nums[3];
				dos.writeInt(ans);
			}
			if (opcion == 4) {
				ans = nums[0] - (nums[1] + nums[2] + nums[3]);
				dos.writeInt(ans);
			}
			if (opcion == 5) {
				ans = nums[0] * nums[1];
				dos.writeInt(ans);
			}
			if (opcion == 6) {
				ans = nums[0] / nums[1];
				int resto = nums[0] % nums[1];
				dos.writeInt(ans);
				dos.writeInt(resto);
			}

			s1.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

}
