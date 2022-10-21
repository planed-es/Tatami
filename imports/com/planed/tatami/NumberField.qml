import QtQuick 2.12

TextField {
  id: root
  property int decimals: 3
  property real min: -Infinity
  property real max: Infinity
  property double number

  validator: DoubleValidator {
    bottom:   root.min
    top:      root.max
    decimals: root.decimals
    locale: "en"
  }

  onTextChanged: {
    const asNumber = parseFloat(text.replace(',', '.'));

    if (min < 0 && text === '-')
      return ;
    if (number !== asNumber)
      number = asNumber;
  }

  onNumberChanged: {
    const asText = isNaN(number) ? '' : number.toString();

    if (text !== asText)
      text = asText;
  }
}
