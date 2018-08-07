# polls

The goal of this project is to create a modular and open architecture for interactive radio campaigns. Interactive radio adds participatory elements to radio through the use of mobile phones and other communication technologies. Some of the channels over which this communication can take place are:
* IVR
* SMS
* Chatbots
* Speech-to-text
<!-- between campaign features and an audience -->
A *channel* is a means for uni- or bidirectional communication with an audience. We think of these channels as interfaces whose operations are determined by the capabilities of the channel:
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
Information available for exchange over a channel is know as *content*. Content comes in different *formats*, and in different *languages*. Examples of formats are text, audio, and video. An instance of data which renders content in a specific format and in a specific language is referred to as a *representation* of that content.
```d
// pseudo-code
class ContentRep
{
    // ...

    private ContentFormat format;
    private Language lang;
}

class Content
{
    // ...

    private ContentRep[] representations;
}
```
### Media types
Content formats conform to the RFC 6838 [Media Type](https://www.iana.org/assignments/media-types/media-types.xhtml) (also know as MIME type) specification.
```d
// pseudo-code
enum MediaType
{
    AudioMpeg,
    TextPlain,
    TextHtml,

    // ...
}

interface ContentFormat
{
    static pure MediaType mediaType();
}

class TextFormat : ContentFormat
{
    static pure MediaType mediaType() 
    { 
        return MediaType.TextPlain; 
    }
}

class Mp3Format : ContentFormat
{
    static pure MediaType mediaType() 
    { 
        return MediaType.AudioMpeg; 
    }
}
```
Interactive radio *campaigns* are schedulable and automated activities to exchange information (content) with the audience.

*Features* are configurable units of functionality that can be added to campaigns. A channel provides the communication link between the audience and a campaign feature. Multiple channels may be available for one feature.
