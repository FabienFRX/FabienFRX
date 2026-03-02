package com.whanos;

public class App {
    public static void main(String[] args) {
        System.out.println("Whanos Java Test Application");
        System.out.println("============================\n");

        while (true) {
            System.out.println("Hello from Whanos Java app! Running...");
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }
        }
    }
}
