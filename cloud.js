const functions = require('firebase-functions');
const admin = require('firebase-admin');
const twilio = require('twilio');
admin.initializeApp();

const twilioSid = functions.config().twilio.sid;
const twilioToken = functions.config().twilio.token;
const twilioNumber = functions.config().twilio.number;
const recipientNumber = functions.config().twilio.receiver;
const client = new twilio(twilioSid, twilioToken);

exports.smsAlertOnIntruder = functions.database
  .ref('/alarmSystem/alerts/{pushId}')
  .onCreate((snapshot, context) => {
    const data = snapshot.val();
    const messageText = `ALERT: Intruder detected!\nPIR:${data.pir} S:${data.sound} IR:${data.ir} LDR:${data.ldr}\nArmed:${data.armed}`;
    return client.messages.create({
      body: messageText,
      from: twilioNumber,
      to: recipientNumber
    });
  });
