using System.IO;
using System.Windows.Media.Imaging;
using System.Windows;

namespace DW3ArduinoEditor.Graphics
{
   public class StaticSpriteTexturePool : ITextureImageProvider
   {
      private readonly List<WriteableBitmap> _staticSpriteTextureBitmaps = [];
      private readonly List<Sprite> _staticSpriteTextureSprites = [];
      private readonly Palette _palette = new();

      public List<List<int>> StaticSpritePaletteIndexes = [];

      public StaticSpriteTexturePool() { }

      public StaticSpriteTexturePool( string imagePath, Palette palette )
      {
         _palette = palette;

         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];

         BitmapUtils.CheckStaticSpriteTexturePoolBitmapFormat( bitmapSource );
         ReadStaticSpriteTextureBitmaps( bitmapSource );
         UpdatePalette( bitmapSource.PixelWidth / Constants.StaticSpriteTextureSize );

         // Extract the static sprites as C# sprites
         var staticSpriteSheet = Sprite.LoadFromFile( imagePath );
         int staticSpriteCount = staticSpriteSheet.Width / Constants.StaticSpriteTextureSize;

         for ( int textureIndex = 0; textureIndex < staticSpriteCount; textureIndex++ )
         {
            int srcX = textureIndex * Constants.StaticSpriteTextureSize;
            int srcY = 0;

            var staticSpriteSprite = staticSpriteSheet.Extract( srcX, srcY, Constants.StaticSpriteTextureSize, Constants.StaticSpriteTextureSize );
            _staticSpriteTextureSprites.Add( staticSpriteSprite );
         }
      }

      // ITextureImageProvider
      public BitmapSource GetImageFromIndex( uint index ) => _staticSpriteTextureBitmaps[(int)index];
      public Sprite GetSpriteFromIndex( uint index ) => _staticSpriteTextureSprites[(int)index];

      private void ReadStaticSpriteTextureBitmaps( BitmapSource bitmapSource )
      {
         for ( int i = 0; i < ( bitmapSource.PixelWidth / Constants.StaticSpriteTextureSize ); i++ )
         {
            int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
            var data = new byte[stride * bitmapSource.PixelHeight];
            bitmapSource.CopyPixels( data, stride, 0 );
            _staticSpriteTextureBitmaps.Add( new WriteableBitmap( Constants.StaticSpriteTextureSize, bitmapSource.PixelHeight, bitmapSource.DpiX, bitmapSource.DpiY, bitmapSource.Format, bitmapSource.Palette ) );
            _staticSpriteTextureBitmaps[i].WritePixels( new Int32Rect( 0, 0, Constants.StaticSpriteTextureSize, Constants.StaticSpriteTextureSize ), data, stride, Constants.StaticSpriteTextureSize * i );
         }
      }

      private void UpdatePalette( int staticSpriteTextureCount )
      {
         int staticSpritePixels = Constants.StaticSpriteTextureSize * Constants.StaticSpriteTextureSize;

         for ( int i = 0; i < staticSpriteTextureCount; i++ )
         {
            StaticSpritePaletteIndexes.Add( new( staticSpritePixels ) );

            for ( int j = 0; j < staticSpritePixels; j++ )
            {
               StaticSpritePaletteIndexes[i].Add( 0 );
            }
         }

         for ( int i = 0; i < staticSpriteTextureCount; i++ )
         {
            var staticSpriteBitmap = _staticSpriteTextureBitmaps[i];

            for ( int y = 0; y < staticSpriteBitmap.PixelHeight; y++ )
            {
               for ( int x = 0; x < staticSpriteBitmap.PixelWidth; x++ )
               {
                  var pixelColor = ColorUtils.GetPixelColor( staticSpriteBitmap, x, y );
                  var pixelColor16 = ColorUtils.ColorToUInt16( pixelColor );
                  _palette.AddColor( pixelColor16 );
                  StaticSpritePaletteIndexes[i][( y * Constants.StaticSpriteTextureSize ) + x] = _palette.GetIndexForColor( pixelColor16 );
               }
            }
         }
      }
   }
}
