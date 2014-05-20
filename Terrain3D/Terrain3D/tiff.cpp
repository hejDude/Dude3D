#include <iostream.h>
#include <fstream.h>

//converts a 4-byte LSB order array to ULong
unsigned long lsbToUL(unsigned char *buff)
{
    unsigned long val=0;
    int i=0;

    for(i=3;i>0;i--)
        val=(val+buff[i])<<8;

    val=val+buff[0];

    return val;
}

//converts a 2-byte LSB order array to UInt
unsigned int lsbToUI(unsigned char *buff)
{
   unsigned int val=0;

   val=val+buff[1];
   val=val<<8;
   val=val+buff[0];

   return val;
}

//reads the resolution values of the first image of a TIFF file
void tiffRes(char *path, double *xRes, double *yRes, int *unit)
{
    unsigned int XRES=0x011A; //TIFF XResolution tag
    unsigned int YRES=0x011B; //TIFF YResolution tag
    unsigned int UNIT=0x0128; //TIFF Resolution Unit tag

    unsigned char buff[5];
    unsigned long seekAddr=0; 
    unsigned int tag=0;

    unsigned long xAddr=0,yAddr=0;
    unsigned int resUnit=0;

    ifstream f;

    f.open(path,ios::binary);
    if(f.fail())
    {
       cout<<"File error"<<endl;
       return;
    }

    f.get(buff,3);            //reads the byte order

    if(buff[0]==0x49 && buff[1]==0x49)
    {
       //this example treats the LSB case

       f.seekg(4);            //seeks the offset of first IFD from header
       f.get(buff,5);         //reads the address of the first IFD
       seekAddr=lsbToUL(buff);
       seekAddr=seekAddr+2;
       f.seekg(seekAddr);     //seeks the addr of the first TIFF tag
                      //skipping the no. of directory entries as it is not used
       f.get(buff,3);         //reads the first TIFF tag
       tag=lsbToUI(buff);

       //reading the file for XRES,YRES and UNIT TIFF tags
       //should be a better way of doing this
       while(xAddr==0 || yAddr==0 || resUnit==0)
       {
           if(tag==UNIT || tag==XRES || tag==YRES)
           {
               if(tag==UNIT)
               {
               f.seekg(seekAddr+8);
               f.get(buff,3);         //read the Resolution Unit
               resUnit=lsbToUI(buff);
               *unit=resUnit;
               }
               else
               {
               f.seekg(seekAddr+8);      
               f.get(buff,5);           //reads the address of the XRes or YRes
               if(tag==XRES)
                   xAddr=lsbToUL(buff); //XRes address
               else
                   yAddr=lsbToUL(buff); //YRes address
               }
           }
           seekAddr=seekAddr+12;           //computes the address of next IFD
           f.seekg(seekAddr);              //seeks the next IFD
           f.get(buff,3);                  //reads the next tag
           tag=lsbToUI(buff);
       }
       //actual reading of the resolution values
       //X resolution
       f.seekg(xAddr);
       f.get(buff,5);
       *xRes=lsbToUL(buff);
       f.seekg(xAddr+4);
       f.get(buff,5);
       *xRes=*xRes/(double)lsbToUL(buff);
       //Y resolution
       f.seekg(yAddr);
       f.get(buff,5);
       *yRes=lsbToUL(buff);
       f.seekg(yAddr+4);
       f.get(buff,5);
       *yRes=*yRes/(double)lsbToUL(buff);

       f.close();
    }
    else
    {
    //Not covered by the example
        f.close();
        if(buff[0]==0x4D && buff[1]==0x4D)
           cout<<"MSB mode used. Not covered by the example..."<<endl;
        else
           cout<<"Unkown byte order..."<<endl;
    }
}

void getResolution()
{
    double *xRes=new double;
    double *yRes=new double;
    int *unit=new int;

    typedef char string[8];
    string resUnit[4]={"","NO_UNIT","INCH","CM"};

    char *path="test.tif";
    *xRes=0;
    *yRes=0;
    *unit=0;

    tiffRes(path,xRes,yRes,unit);

    cout<<"UNIT:  "<<resUnit[(*unit)]<<endl;
    cout<<"X res: "<<(*xRes)<<endl;
    cout<<"Y res: "<<(*yRes)<<endl;
}
