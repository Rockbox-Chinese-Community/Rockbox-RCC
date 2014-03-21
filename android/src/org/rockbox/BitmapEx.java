/*
 * BimtapEx for Android to write the Bitmap as windows BMP file 
 * 
 * Author: Kenny S.W. Lee <kswlee@gmail.com> kennylee.co 
 * Created: 2013.07.11 
 * Reference: http://thiscouldbebetter.wordpress.com/2011/08/16/reading-and-writing-a-bmp-file-using-java/ 
 * 		The original ImageBMP code failed to be used on android, so I did some minor changes
 * 			1. raw pixels serialization with correct order conversion 
 * 			2. make it workable with Android's existing Bitmap object
 * 
 * Sample Usage: 
 * 			BitmapEx bmpEx = new BitmapEx(Bitmap.createBitmap(320, 320, Bitmap.Config.ARGB_8888));
 *			bmpEx.saveAsBMP(new FileOutputStream("/sdcard/test.bmp"));
 */
package org.rockbox;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import android.graphics.Bitmap;
public class BitmapEx {
	private static final int FILE_HEADER_SIZE = 14;
	private static final int DIB_HEADER_SIZE = 40;
	private static final int BI_RGB = 0;
	private static final int [] PIXELS_PER_METER = new int[] {2835, 2835};
	
	private Bitmap _bmp;	
	public BitmapEx(Bitmap bmp) {
		_bmp = bmp;		
	}
	
	public Bitmap getBitmap() {
		return _bmp;
	}
	
	public void saveAsBMP(OutputStream stream) throws IOException {		
		int fileSize = (FILE_HEADER_SIZE + DIB_HEADER_SIZE + 4 * _bmp.getWidth() * _bmp.getHeight());
		FileHeader fileHeader = new FileHeader("BM", fileSize, (short)0, (short)0, FILE_HEADER_SIZE + DIB_HEADER_SIZE);
		
		DataOutputStream dos = new DataOutputStream(stream);
		DataOutputStreamLittleEndian dosl = new DataOutputStreamLittleEndian(dos);
		fileHeader.write(dosl);
		
		DIBHeader dibHeader = new DIBHeaderBitmapInfo (
					// dibHeaderSize;					      
                    // imageSizeInPixelsXY
					// planes;
                	// bitsPerPixel;
                	// compression; // 0: BI_RGB
                	// imageSizeInBytes;
                	// pixelsPerMeterXY
                	// numberOfColorsInPalette
                	// numberOfColorsUsed				
					DIB_HEADER_SIZE,
                    new int[] {
                        _bmp.getWidth(),
                        _bmp.getHeight(),
                    },
                    (short)1, 
                    (short)32,
                    BI_RGB,  
                    _bmp.getWidth() * _bmp.getHeight(),
                    PIXELS_PER_METER,
                    0,
                    0 
                );
		
		dibHeader.writeToStream(dosl);
		
		// Serialize the pixels
		int width = _bmp.getWidth();
		int [] pixels = new int[width];		
		for (int h = _bmp.getHeight() - 1; h >= 0; h--) { // reverse scan line		
			_bmp.getPixels(pixels, 0, width, 0, h, width, 1);			
			for (int i = 0; i < width; ++i) 
				pixels[i] = ByteOrder.reverse(pixels[i]); // reverse ARGB as BGRA			
			dosl.write(pixels);					
		}				
		dosl.close();
	}
	
	/*
	 * BMP FileHeader
	 */
	private class FileHeader { // 14 bytes
        public String signature;
        public int fileSize;
        public short reserved1;
        public short reserved2;
        public int fileOffsetToPixelArray;
        
        public FileHeader(String signature, 
        		int fileSize, 
        		short reserved1, 
        		short reserved2, 
        		int fileOffset) {
        	
        	this.signature = signature;
        	this.fileSize = fileSize;
        	this.reserved1 = reserved1;
        	this.reserved2 = reserved2;
        	this.fileOffsetToPixelArray = fileOffset;        	
        }
        
        public void write(DataOutputStreamLittleEndian dataOutputStream) throws IOException {
        	dataOutputStream.writeString(signature);
        	dataOutputStream.writeInt(fileSize);
        	dataOutputStream.writeShort(reserved1);
        	dataOutputStream.writeShort(reserved2);
        	dataOutputStream.writeInt(fileOffsetToPixelArray);
        }
	}
	
	/*
	 * BMP DIBHeader 
	 */
    public static abstract class DIBHeader {
        public String name;
        public int sizeInBytes;

        public DIBHeader(String name, int sizeInBytes) {
            this.name = name;
            this.sizeInBytes = sizeInBytes;
        }        
         
        public abstract void writeToStream(DataOutputStreamLittleEndian reader);
    }

    public static class DIBHeaderBitmapInfo extends DIBHeader
    {
        public int[] imageSizeInBytesXY;
        public short planes;
        public short bitsPerPixel;
        public int compression;
        public int imageSizeInBytes;
        public int[] pixelsPerMeterXY;
        public int numberOfColorsInPalette;
        public int numberOfColorsUsed;

        public int[] imageSizeInPixelsXY;

        public DIBHeaderBitmapInfo() {
            super("BitmapInfo", DIB_HEADER_SIZE);
        }

        public DIBHeaderBitmapInfo (
            int sizeInBytes,
            int[] imageSizeInPixelsXY,
            short planes,
            short bitsPerPixel,
            int compression,
            int imageSizeInBytes,
            int[] pixelsPerMeterXY,
            int numberOfColorsInPalette,
            int numberOfColorsUsed) {
        	
            this();
            this.sizeInBytes = sizeInBytes;
            this.imageSizeInPixelsXY = imageSizeInPixelsXY;
            this.planes = planes;
             this.bitsPerPixel = bitsPerPixel;
            this.compression = compression;
            this.imageSizeInBytes = imageSizeInBytes;    
            this.pixelsPerMeterXY = pixelsPerMeterXY;
            this.numberOfColorsInPalette = numberOfColorsInPalette;
            this.numberOfColorsUsed = numberOfColorsUsed;

            if (this.imageSizeInBytes == 0) {
                this.imageSizeInBytes =
                    this.imageSizeInBytesXY[0]
                    * this.imageSizeInBytesXY[1]
                    * this.bitsPerPixel
                    / 8;
            }
        }

        public void writeToStream(DataOutputStreamLittleEndian writer) {
            try {
                writer.writeInt(this.sizeInBytes);
                writer.writeInt(this.imageSizeInPixelsXY[0]);
                writer.writeInt(this.imageSizeInPixelsXY[1]);

                writer.writeShort(this.planes);
                writer.writeShort(this.bitsPerPixel);
                writer.writeInt(this.compression);
                writer.writeInt(this.imageSizeInBytes);

                writer.writeInt(this.pixelsPerMeterXY[0]);
                writer.writeInt(this.pixelsPerMeterXY[1]);

                writer.writeInt(numberOfColorsInPalette);
                writer.writeInt(numberOfColorsUsed);
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
    }	
}

class DataOutputStreamLittleEndian
{
    private DataOutputStream systemStream;
    
    public DataOutputStreamLittleEndian(DataOutputStream systemStream)
    {
        this.systemStream = systemStream;
    }

    public void close() throws IOException
    {
        this.systemStream.close();
    }

    public void write(byte[] bytesToWriteFrom) throws IOException
    {
        this.systemStream.write(bytesToWriteFrom);
    }
    
    public void write(int [] pixels) throws IOException 
    {    	
		ByteBuffer byteBuffer = ByteBuffer.allocate(pixels.length * 4);        
        IntBuffer intBuffer = byteBuffer.asIntBuffer();
        intBuffer.put(pixels);
        byte[] array = byteBuffer.array();
        write(array);
        array = null;
        byteBuffer = null;
    }

    public void writeInt(int valueToWrite) throws IOException
    {
        this.systemStream.writeInt(ByteOrder.reverse(valueToWrite));
    }

    public void writeLong(long valueToWrite) throws IOException
    {
        this.systemStream.writeLong(ByteOrder.reverse(valueToWrite));
    }

    public void writeShort(short valueToWrite) throws IOException
    {
        this.systemStream.writeShort(ByteOrder.reverse(valueToWrite));
    }

    public void writeString(String stringToWrite) throws IOException
    {
        this.systemStream.writeBytes(stringToWrite);
    }
}

class ByteOrder
{
    public static void reverse(byte[] bytesToConvert)
    {
        int numberOfBytes = bytesToConvert.length;
        int numberOfBytesHalf = numberOfBytes / 2;

        for (int b = 0; b < numberOfBytesHalf; b++)
        {
            byte byteFromStart = bytesToConvert[b];
            bytesToConvert[b] = bytesToConvert[numberOfBytes - 1 - b];
            bytesToConvert[numberOfBytes - 1 - b] = byteFromStart;
        }
    }

    public static int reverse(int intToReverse)
    {
        byte[] intAsBytes = new byte[]
        {
            (byte)(intToReverse & 0xFF),
            (byte)((intToReverse >> 8 ) & 0xFF),
            (byte)((intToReverse >> 16) & 0xFF),
            (byte)((intToReverse >> 24) & 0xFF),
        };

        intToReverse =
        (
            (intAsBytes[3] & 0xFF)
            + ((intAsBytes[2] & 0xFF) << 8 )
            + ((intAsBytes[1] & 0xFF) << 16)
            + ((intAsBytes[0] & 0xFF) << 24)
        );

        return intToReverse;
    }

    public static long reverse(long valueToReverse)
    {
        byte[] valueAsBytes = new byte[]
        {
            (byte)(valueToReverse & 0xFF),
            (byte)((valueToReverse >> 8 ) & 0xFF),
            (byte)((valueToReverse >> 16) & 0xFF),
            (byte)((valueToReverse >> 24) & 0xFF),
            (byte)((valueToReverse >> 32) & 0xFF),
            (byte)((valueToReverse >> 40) & 0xFF),
            (byte)((valueToReverse >> 48 ) & 0xFF),
            (byte)((valueToReverse >> 56) & 0xFF),
        };

        long returnValue = (valueAsBytes[7] & 0xFF);
        returnValue += ((valueAsBytes[6] & 0xFF) << 8 );
        returnValue += ((valueAsBytes[5] & 0xFF) << 16);
        returnValue += ((valueAsBytes[4] & 0xFF) << 24);
        returnValue += ((valueAsBytes[3] & 0xFF) << 32);
        returnValue += ((valueAsBytes[2] & 0xFF) << 40);
        returnValue += ((valueAsBytes[1] & 0xFF) << 48 );
        returnValue += ((valueAsBytes[0] & 0xFF) << 56);

        return returnValue;
    }

    public static short reverse(short valueToReverse)
    {
        byte[] valueAsBytes = new byte[]
        {
            (byte)(valueToReverse & 0xFF),
            (byte)((valueToReverse >> 8 ) & 0xFF),
        };

        valueToReverse = (short)
        (
            ((valueAsBytes[1] & 0xFF))
            + ((valueAsBytes[0] & 0xFF) << 8 )
        );

        return valueToReverse;
    }
}
