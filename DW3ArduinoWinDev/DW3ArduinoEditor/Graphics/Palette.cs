namespace DW3ArduinoEditor.Graphics
{
   public class Palette
   {
      public List<UInt16> Colors { get; } = new( Constants.PaletteMaxColors );
      public int ColorCount { get; private set; } = 0;

      public Palette()
      {
         for ( int i = 0; i < Constants.PaletteMaxColors; i++ )
         {
            Colors.Add( 0 );
         }
      }

      public void AddColor( UInt16 color )
      {
         if ( GetIndexForColor( color ) < 0 )
         {
            ColorCount++;

            if ( ColorCount > Constants.PaletteMaxColors )
            {
               throw new Exception( "Palette contains too many colors" );
            }

            Colors[ColorCount - 1] = color;
         }
      }

      public int GetIndexForColor( UInt16 color16 )
      {
         for ( int i = 0; i < ColorCount; i++ )
         {
            if ( Colors[i] == color16 )
            {
               return i;
            }
         }

         return -1;
      }
   }
}
