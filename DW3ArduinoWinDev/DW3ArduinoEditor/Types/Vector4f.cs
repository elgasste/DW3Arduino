namespace DW3ArduinoEditor.Types
{
   public class Vector4f
   {
      public float X { get;set; }
      public float Y { get;set; }
      public float W { get;set; }
      public float H { get;set; }

      public Vector4f() { }

      public Vector4f( float x, float y, float w, float h )
      {
         X = x;
         Y = y;
         W = w;
         H = h;
      }
   }
}
