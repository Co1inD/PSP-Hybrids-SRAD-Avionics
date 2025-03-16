package net.treecaptcha.psp;

import net.treecaptcha.psp.decoder.MessagePacket;
import net.treecaptcha.psp.decoder.PacketData;
import net.treecaptcha.psp.decoder.UpdatePacket;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.dial.*;
import org.jfree.data.general.DefaultValueDataset;
import org.jfree.data.general.ValueDataset;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;

public class MainWindow extends JPanel{
    public JTabbedPane tabbedPane1;
    private JPanel contentPane;
    private JTextField commandField;
    private JTable table1;
    private JPanel velocityGauge;
    private JPanel accelerationGauge;
    private JPanel altitudeGraph;
    private JTextPane logPane;
    private JScrollPane logScrollPane;


    // Display datasets
    private DefaultValueDataset velocity;
    private DefaultValueDataset acceleration;
    private int latest_packet_timestamp;
    private String[][] tableData;

    // Listener
    private OnCommandReceive receiver;

    public void update(PacketData packetData){
        if (packetData instanceof UpdatePacket){
            UpdatePacket updatePacket = (UpdatePacket) packetData;
            if (latest_packet_timestamp < updatePacket.time){
                velocity.setValue(length(updatePacket.velocityE, updatePacket.velocityN, updatePacket.velocityU));
                acceleration.setValue(length(updatePacket.accelerationE, updatePacket.accelerationN, updatePacket.accelerationU));
                tableData[3][1] = String.valueOf(updatePacket.accelerationN);
                tableData[3][2] = String.valueOf(updatePacket.accelerationE);
                tableData[3][3] = String.valueOf(updatePacket.accelerationU);

                tableData[4][1] = String.valueOf(updatePacket.velocityN);
                tableData[4][2] = String.valueOf(updatePacket.velocityE);
                tableData[4][3] = String.valueOf(updatePacket.velocityU);

                tableData[5][1] = String.valueOf(updatePacket.positionN);
                tableData[5][2] = String.valueOf(updatePacket.positionE);
                tableData[5][3] = String.valueOf(updatePacket.positionU);

                tableData[8][1] = String.valueOf(updatePacket.polar);
                tableData[8][2] = String.valueOf(updatePacket.azimuth);

                if (!updatePacket.isReady)
                    tableData[0][1] = "Not Armed";
                if (updatePacket.isReady)
                    tableData[0][1] = "Armed";
                if (updatePacket.hasLaunched)
                    tableData[0][1] = "Boost";
                if (updatePacket.hasEndedBoost)
                    tableData[0][1] = "Coast";
                if (updatePacket.hasLaunchedFirstParachute)
                    tableData[0][1] = "First Parachute";
                if (updatePacket.hasLaunchedSecondParachute)
                    tableData[0][1] = "Second Parachute";
                updateTable();
            }
        }
        if (packetData instanceof MessagePacket){
            MessagePacket logPacket = (MessagePacket) packetData;
            displayLog(": " + logPacket.text);
        }

    }


    public MainWindow(OnCommandReceive commandReceiver) {
        contentPane = new JPanel();
        JFrame frame = new JFrame("MainWindow");
        frame.setContentPane(this.contentPane);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        frame.setVisible(true);


        velocity = new DefaultValueDataset(-1);
        JFreeChart velocityDialChart = createStandardDialChart("Velocity", "magnitude (m/s)", velocity, 0, 500, 50, 9);
        ChartPanel velocityChartPanel = new ChartPanel(velocityDialChart);
        velocityGauge.setLayout(new java.awt.BorderLayout());
        velocityGauge.add(velocityChartPanel, BorderLayout.CENTER);

        acceleration = new DefaultValueDataset(-1);
        JFreeChart accelerationDialChart = createStandardDialChart("Acceleration", "magnitude (m/s^2)", acceleration, 0, 100, 10, 9);
        ChartPanel chartPanel = new ChartPanel(accelerationDialChart);
        accelerationGauge.setLayout(new java.awt.BorderLayout());
        accelerationGauge.add(chartPanel, BorderLayout.CENTER);

        DefaultTableModel model = new DefaultTableModel(9, 4);
        table1.setModel(model);
        table1.setValueAt("No Data", 0, 0);
        table1.setRowHeight(30);

        tableData = new String[][]{
                {"Current Phase", "No Data", "Time Since Last Update", "No Data"},
                {"", "", "", ""},
                {"", "North", "East", "Up"},
                {"Acceleration (m/s^2)", "No Data", "No Data", "No Data"},
                {"Velocity (m/s)", "No Data", "No Data", "No Data"},
                {"Position (m)", "No Data", "No Data", "No Data"},
                {"", "", "", ""},
                {"", "Polar", "Azimuthal", ""},
                {"Orientation (degrees)", "No Data", "No Data", ""},


        };
        updateTable();

        // When command is issued pass it on
        receiver = commandReceiver;
        Action action = new AbstractAction() {

            @Override
            public void actionPerformed(ActionEvent e) {
                String command = commandField.getText();
                commandField.setText("");
                if (command.isBlank()) return;
                if (receiver != null) receiver.onCommand(command);
            }
        };
        commandField.addActionListener(action);

        frame.setExtendedState(JFrame.MAXIMIZED_BOTH);
        commandField.requestFocus();
    
    }
    private void updateTable(){
        for (int i = 0; i < tableData.length; i++){
            for (int j = 0; j < tableData[i].length; j++){
                table1.setValueAt(tableData[i][j], i, j);
            }
        }
    }
    public static void main(String[] args) throws InterruptedException {

        MainWindow mainWindow = new MainWindow(null);
        for (int i = 0; i < 20; i++) {
            mainWindow.displayLog("UI TEST LOG " + i);
            Thread.sleep(500);
        }

    }
    public static JFreeChart createStandardDialChart(String title, String miniTitle, ValueDataset valuedataset, double min, double max, double step, int minorTicks)
    {
        DialPlot dialplot = new DialPlot();
        dialplot.setDataset(valuedataset);
        dialplot.setDialFrame(new StandardDialFrame());
        dialplot.setBackground(new DialBackground());
        DialTextAnnotation dialtextannotation = new DialTextAnnotation(miniTitle);
        dialtextannotation.setFont(new Font("Dialog", 1, 14));
        dialtextannotation.setRadius(0.69999999999999996D);
        dialplot.addLayer(dialtextannotation);
        DialValueIndicator dialvalueindicator = new DialValueIndicator(0);
        dialplot.addLayer(dialvalueindicator);
        StandardDialScale standarddialscale = new StandardDialScale(min, max, -120D, -300D, 10D, 4);
        standarddialscale.setMajorTickIncrement(step);
        standarddialscale.setMinorTickCount(minorTicks);
        standarddialscale.setTickRadius(0.88D);
        standarddialscale.setTickLabelOffset(0.14999999999999999D);
        standarddialscale.setTickLabelFont(new Font("Dialog", 0, 14));
        dialplot.addScale(0, standarddialscale);
        dialplot.addPointer(new org.jfree.chart.plot.dial.DialPointer.Pin());
        DialCap dialcap = new DialCap();
        dialplot.setCap(dialcap);
        return new JFreeChart(title, dialplot);
    }


    private double length(double x, double y, double z){
        return Math.sqrt(Math.pow(x, 2) + Math.pow(y, 2) + Math.pow(z, 2));
    }

    public void displayLog(String log){
        // Check if at bottom of scroll pane
        Point position = logScrollPane.getViewport().getViewPosition();
        Dimension viewSize = logScrollPane.getViewport().getViewSize();

        // if within 30 of bottom, scroll
        boolean autoScroll = -30<(position.getY() - viewSize.getHeight() + logScrollPane.getSize().getHeight());

        // Append text
        logPane.setText(logPane.getText() + log + "\n");

        // If was at bottom, make at bottom
        if (autoScroll){
            JScrollBar vertical = logScrollPane.getVerticalScrollBar();
            vertical.setValue(vertical.getMaximum());
        }
    }
    private void createUIComponents() {
        // TODO: place custom component creation code here
    }
}
