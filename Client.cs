using MQTTnet.Client;
using System.Text;

var mqttFactory = new MQTTnet.MqttFactory();
var mqttClient = mqttFactory.CreateMqttClient();

using (mqttClient){
    var mqttClientOptions = new MqttClientOptionsBuilder()
                .WithTcpServer(MqttConfig.Server, MqttConfig.Port)
                .Build();
            
    // Message handler
    mqttClient.ApplicationMessageReceivedAsync += e =>
    {
        Console.WriteLine("RECEIVED MESSAGE");
        string payLoad = Encoding.UTF8.GetString(e.ApplicationMessage.Payload);
        Console.WriteLine($"+ Payload = PIN {payLoad}");
        return Task.CompletedTask;
    };

    try
    {
        // connecting the client to the server
        await mqttClient.ConnectAsync(mqttClientOptions, CancellationToken.None);
    }
    catch (Exception e)
    {
        Console.WriteLine(e);
    }

    try
    {
        string topic = "data";
        // subscribing to a topic
        var mqttSubscribeOptions = mqttFactory.CreateSubscribeOptionsBuilder()
            .WithTopicFilter(f => f.WithTopic(topic))
            .Build();

        await mqttClient.SubscribeAsync(mqttSubscribeOptions, CancellationToken.None);

        Console.WriteLine("Subscribed to topic: "+topic);

        Console.ReadLine();                           
    }

    catch (Exception e)
    {
        Console.WriteLine(e);
    }

}

static class MqttConfig
    {
        public static readonly string Server = "127.0.0.1"; // localhost
        public static readonly int Port = 1883;
    }

