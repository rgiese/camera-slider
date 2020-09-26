import { Buffer } from "buffer";

const encoderRing = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/".split("");

const decoderRing = new Uint8Array(256);
{
  for (var idxEncoded = 0; idxEncoded < encoderRing.length; ++idxEncoded) {
    decoderRing[encoderRing[idxEncoded].charCodeAt(0)] = idxEncoded;
  }
}

const decodedBytesPerOutputGroup = 3;
const encodedBytesPerOutputGroup = 4;
const decodedBitsPerEncodedByte = (8 * decodedBytesPerOutputGroup) / encodedBytesPerOutputGroup;

const encoderRingSize = encoderRing.length;
const encodedTailPadding = "=";

export function Base64Encode(data: Uint8Array): string {
  let accumulator = 0;
  let bytesAccumulated = 0;
  let asString = "";

  const accumulateByte = (currentValue: number): void => {
    accumulator = ((accumulator << 8) + currentValue) >>> 0; // >>> -> coerce as uint32
    ++bytesAccumulated;

    if (bytesAccumulated % decodedBytesPerOutputGroup === 0) {
      let divisor = Math.pow(encoderRingSize, decodedBytesPerOutputGroup);

      while (divisor >= 1) {
        const encoderRingIndex = Math.floor(accumulator / divisor) % encoderRingSize;
        const encodedCharacter = encoderRing[encoderRingIndex];

        asString = asString.concat(encodedCharacter);

        divisor /= encoderRingSize;
      }

      accumulator = 0;
    }
  };

  data.forEach(currentValue => accumulateByte(currentValue));

  let nPaddingCharacters = 0;

  while (bytesAccumulated % decodedBytesPerOutputGroup !== 0) {
    // Tail padding of zeroes (need to flush buffer; will replace below)
    accumulateByte(0);

    ++nPaddingCharacters;
  }

  if (nPaddingCharacters) {
    asString = asString.slice(0, -nPaddingCharacters);
    asString += encodedTailPadding.repeat(nPaddingCharacters);
  }

  return asString;
}

export function Base64EncodeInt32(value: number): string {
  const buffer = Buffer.allocUnsafe(4);
  buffer.writeInt32LE(value, 0);

  return Base64Encode(new Uint8Array(buffer));
}

export function Base64EncodeUInt32(value: number): string {
  const buffer = Buffer.allocUnsafe(4);
  buffer.writeUInt32LE(value, 0);

  return Base64Encode(new Uint8Array(buffer));
}

export function Base64Decode(source: string): Uint8Array {
  if (source.length % encodedBytesPerOutputGroup !== 0) {
    throw new Error(
      `Base64 string ${source} length not a multiple of ${encodedBytesPerOutputGroup}`
    );
  }

  // Allocate maximum array length
  const asByteArray = new Uint8Array(
    (source.length / encodedBytesPerOutputGroup) * decodedBytesPerOutputGroup
  );

  let accumulator = 0;
  let bytesAccumulated = 0;
  let bytesStored = 0;
  let paddingBytes = 0;

  const accumulateByte = (currentValue: number): void => {
    accumulator = ((accumulator << decodedBitsPerEncodedByte) + currentValue) >>> 0; // >>> -> coerce as uint32
    ++bytesAccumulated;

    if (bytesAccumulated % encodedBytesPerOutputGroup === 0) {
      for (let nBytesStored = 0; nBytesStored < decodedBytesPerOutputGroup; ++nBytesStored) {
        const groupRelativeByteIndex = decodedBytesPerOutputGroup - nBytesStored - 1; // Reverse byte order

        asByteArray[bytesStored + groupRelativeByteIndex] = accumulator & 0xff;

        accumulator = accumulator >>> 8;
      }

      bytesStored += decodedBytesPerOutputGroup;
    }
  };

  for (let sourceIdx = 0; sourceIdx < source.length; ++sourceIdx) {
    if (source.charAt(sourceIdx) === encodedTailPadding) {
      ++paddingBytes;
      accumulateByte(0);
    } else {
      accumulateByte(decoderRing[source.charCodeAt(sourceIdx)]);
    }
  }

  return paddingBytes ? asByteArray.slice(0, -paddingBytes) : asByteArray;
}

export function Base64DecodeString(source: string): string {
  const asByteArray = Base64Decode(source);

  if (!asByteArray.length) {
    return "";
  }

  return asByteArray.reduce((output, elem) => output + String.fromCharCode(elem), "");
}

export function Base64DecodeUInt32(source: string): number {
  const asByteArray = Base64Decode(source);

  if (asByteArray.length != 4) {
    return 0;
  }

  return Buffer.from(asByteArray).readUInt32LE();
}

export function Base64DecodeInt32(source: string): number {
  const asByteArray = Base64Decode(source);

  if (asByteArray.length != 4) {
    return 0;
  }

  return Buffer.from(asByteArray).readInt32LE();
}

//
// Tests
//

function base64Tests(): void {
  function test(decoded: string, expectedEncoded: string): void {
    const asByteArray = new Uint8Array(decoded.length);

    for (let sourceIdx = 0; sourceIdx < decoded.length; ++sourceIdx) {
      asByteArray[sourceIdx] = decoded.charCodeAt(sourceIdx);
    }

    const encoded = Base64Encode(asByteArray);

    if (encoded !== expectedEncoded) {
      throw new Error(
        `Base64 encoder test: expected '${decoded}' to encode as '${expectedEncoded}', got '${encoded}'`
      );
    }

    const decodedByteArray = Base64Decode(encoded);
    const decodedString = decodedByteArray.reduce(
      (output, elem) => output + String.fromCharCode(elem),
      ""
    );

    if (decoded !== decodedString) {
      throw new Error(
        `Base64 decoder test: expected '${encoded}' to encode as '${decoded}', got '${decodedString}' (${decodedByteArray.reduce(
          (output, elem) => output + "0x" + elem.toString(16) + " ",
          ""
        )})`
      );
    }
  }

  // Test cases from https://en.wikipedia.org/wiki/Base64
  test("Man", "TWFu");
  test("Ma", "TWE=");
  test("M", "TQ==");

  test("pleasure.", "cGxlYXN1cmUu");
  test("leasure.", "bGVhc3VyZS4=");
  test("easure.", "ZWFzdXJlLg==");
  test("asure.", "YXN1cmUu");
  test("sure.", "c3VyZS4=");
}

base64Tests();
