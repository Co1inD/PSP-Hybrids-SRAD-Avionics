package net.treecaptcha.psp;

import javax.swing.*;
import java.awt.*;

public class MainWindow extends JPanel{

    public static void main(String[] args) {
        JFrame frame = new JFrame("MainWindow");
        frame.setContentPane(new MainWindow().contentPane);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(new Dimension(2048,2048));
        frame.setMinimumSize(new Dimension(400, 256));
        frame.pack();
        frame.setVisible(true);
    }
    public MainWindow(){
        contentPane = new JPanel();
    }
    private JPanel contentPane;
    private JTabbedPane tabbedPane1;
    private JTextPane logTextPane;
    private JPanel logPanel;
    private JComboBox File;
    private JComboBox comboBox2;
    private JTable table1;
    private JPanel Config;
    private JTable currentStatus;

    private void createUIComponents() {
        // TODO: place custom component creation code here
    }
}
