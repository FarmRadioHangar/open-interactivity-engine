# fluffy-umbrella

The goal of this project is to create a modular and open architecture for interactive radio campaigns. Interactive radio adds participatory elements to radio through the use of mobile phones and other communication technologies. Some of the channels over which this communication can take place are:
* IVR
* SMS
* Chatbots
* Speech-to-text
<!-- between campaign features and an audience -->
A *channel* is a means for uni- or bidirectional communication with an audience. We think of these channels as interfaces in which the operations are determined by the capabilities of the channel:
```d
// pseudo-code
interface SmsChannel
{
    void sendSms(Audience recipients, Content message);
}
```
An *adapter* implements a channel interface against a specific backend, service or platform.
```d
// pseudo-code
module nexmo_sms;

class NexmoSmsAdapter : SmsChannel
{
    void sendSms(Audience recipients, Content message)
    {
        // code that talks to Nexmo's API
    }
}
```
