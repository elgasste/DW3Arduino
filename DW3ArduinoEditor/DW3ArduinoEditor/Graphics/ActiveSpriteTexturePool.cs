using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;

namespace DW3ArduinoEditor.Graphics
{
   public class ActiveSpriteTexturePool : ITextureImageProvider
   {
      private readonly List<WriteableBitmap> _activeSpriteTextureBitmaps = [];
      private readonly List<Sprite> _activeSpriteTextureSprites = [];
      private readonly Palette _palette = new();

      public List<List<int>> ActiveSpritePaletteIndexes = [];

      public ActiveSpriteTexturePool() { }

      public ActiveSpriteTexturePool( Palette palette )
      {
         _palette = palette;
      }

      public ActiveSpriteTexturePool( string imagePath, Palette palette )
      {
         _palette = palette;

         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];

         BitmapUtils.CheckActiveSpriteTexturePoolBitmapFormat( bitmapSource );
         ReadActiveSpriteTextureBitmaps( bitmapSource );
         UpdatePalette( bitmapSource.PixelWidth / Constants.ActiveSpriteTextureWidth );

         // Extract the active sprites as C# sprites
         var activeSpriteSheet = Sprite.LoadFromFile( imagePath );
         int activeSpriteCount = activeSpriteSheet.Width / Constants.ActiveSpriteTextureWidth;

         for ( int textureIndex = 0; textureIndex < activeSpriteCount; textureIndex++ )
         {
            int srcX = textureIndex * Constants.ActiveSpriteTextureWidth;
            int srcY = 0;

            var activeSpriteSprite = activeSpriteSheet.Extract( srcX, srcY, Constants.ActiveSpriteTextureWidth, Constants.ActiveSpriteTextureHeight );
            _activeSpriteTextureSprites.Add( activeSpriteSprite );
         }
      }

      // ITextureImageProvider
      public BitmapSource GetImageFromIndex( uint index ) => _activeSpriteTextureBitmaps[(int)index];

      private void ReadActiveSpriteTextureBitmaps( BitmapSource bitmapSource )
      {
         for ( int i = 0; i < ( bitmapSource.PixelWidth / Constants.ActiveSpriteTextureWidth ); i++ )
         {
            int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
            var data = new byte[stride * bitmapSource.PixelHeight];
            bitmapSource.CopyPixels( data, stride, 0 );
            _activeSpriteTextureBitmaps.Add( new WriteableBitmap( Constants.ActiveSpriteTextureWidth, bitmapSource.PixelHeight, bitmapSource.DpiX, bitmapSource.DpiY, bitmapSource.Format, bitmapSource.Palette ) );
            _activeSpriteTextureBitmaps[i].WritePixels( new Int32Rect( 0, 0, Constants.ActiveSpriteTextureWidth, Constants.ActiveSpriteTextureHeight ), data, stride, Constants.ActiveSpriteTextureWidth * i );
         }
      }

      private void UpdatePalette( int activeSpriteTextureCount )
      {
         int activeSpritePixels = Constants.ActiveSpriteTextureWidth * Constants.ActiveSpriteTextureHeight;

         for ( int i = 0; i < activeSpriteTextureCount; i++ )
         {
            ActiveSpritePaletteIndexes.Add( new( activeSpritePixels ) );

            for ( int j = 0; j < activeSpritePixels; j++ )
            {
               ActiveSpritePaletteIndexes[i].Add( 0 );
            }
         }

         for ( int i = 0; i < activeSpriteTextureCount; i++ )
         {
            var activeSpriteBitmap = _activeSpriteTextureBitmaps[i];

            for ( int y = 0; y < activeSpriteBitmap.PixelHeight; y++ )
            {
               for ( int x = 0; x < activeSpriteBitmap.PixelWidth; x++ )
               {
                  var pixelColor = ColorUtils.GetPixelColor( activeSpriteBitmap, x, y );
                  var pixelColor16 = ColorUtils.ColorToUInt16( pixelColor );
                  _palette.AddColor( pixelColor16 );
                  ActiveSpritePaletteIndexes[i][( y * Constants.ActiveSpriteTextureWidth ) + x] = _palette.GetIndexForColor( pixelColor16 );
               }
            }
         }
      }
   }
}
