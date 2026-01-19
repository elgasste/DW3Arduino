using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;

namespace DW3ArduinoEditor.Graphics
{
   public class TileTexturePool : ITextureImageProvider
   {
      private readonly List<WriteableBitmap> _tileTextureBitmaps = [];
      private readonly List<Sprite> _tileTextureSprites = [];
      private readonly Palette _palette = new();

      public List<List<int>> TilePaletteIndexes = [];

      public TileTexturePool() { }

      public TileTexturePool( string imagePath, Palette palette )
      {
         _palette = palette;

         var textFileStream = new FileStream( imagePath, FileMode.Open, FileAccess.Read, FileShare.Read );
         var textDecoder = new PngBitmapDecoder( textFileStream, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.Default );
         var bitmapSource = textDecoder.Frames[0];

         BitmapUtils.CheckTileTexturePoolBitmapFormat( bitmapSource );
         ReadTileTextureBitmaps( bitmapSource );
         UpdatePalette( bitmapSource.PixelWidth / Constants.TileSizePixels );

         // Extract the tiles as sprites
         var tileSheet = Sprite.LoadFromFile( imagePath );
         int tileCount = tileSheet.Width / Constants.TileSizePixels;

         for ( int textureIndex = 0; textureIndex < tileCount; textureIndex++ )
         {
            int srcX = textureIndex * Constants.TileSizePixels;
            int srcY = 0;

            var tileSprite = tileSheet.Extract( srcX, srcY, Constants.TileSizePixels, Constants.TileSizePixels );
            _tileTextureSprites.Add( tileSprite );
         }
      }

      // ITextureImageProvider
      public BitmapSource GetImageFromIndex( uint index ) => _tileTextureBitmaps[(int)index];
      public Sprite GetSpriteFromIndex( uint index ) => _tileTextureSprites[(int)index];

      private void ReadTileTextureBitmaps( BitmapSource bitmapSource )
      {
         for ( int i = 0; i < ( bitmapSource.PixelWidth / Constants.TileSizePixels ); i++ )
         {
            int stride = bitmapSource.PixelWidth * ( bitmapSource.Format.BitsPerPixel / 8 );
            var data = new byte[stride * bitmapSource.PixelHeight];
            bitmapSource.CopyPixels( data, stride, 0 );
            _tileTextureBitmaps.Add( new WriteableBitmap( Constants.TileSizePixels, bitmapSource.PixelHeight, bitmapSource.DpiX, bitmapSource.DpiY, bitmapSource.Format, bitmapSource.Palette ) );
            _tileTextureBitmaps[i].WritePixels( new Int32Rect( 0, 0, Constants.TileSizePixels, Constants.TileSizePixels ), data, stride, Constants.TileSizePixels * i );
         }
      }

      private void UpdatePalette( int tileTextureCount )
      {
         int tilePixels = Constants.TileSizePixels * Constants.TileSizePixels;

         for ( int i = 0; i < tileTextureCount; i++ )
         {
            TilePaletteIndexes.Add( new( tilePixels ) );

            for ( int j = 0; j < tilePixels; j++ )
            {
               TilePaletteIndexes[i].Add( 0 );
            }
         }

         for ( int i = 0; i < tileTextureCount; i++ )
         {
            var tileBitmap = _tileTextureBitmaps[i];

            for ( int y = 0; y < tileBitmap.PixelHeight; y++ )
            {
               for ( int x = 0; x < tileBitmap.PixelWidth; x++ )
               {
                  var pixelColor = ColorUtils.GetPixelColor( tileBitmap, x, y );
                  var pixelColor16 = ColorUtils.ColorToUInt16( pixelColor );
                  _palette.AddColor( pixelColor16 );
                  TilePaletteIndexes[i][( y * Constants.TileSizePixels ) + x] = _palette.GetIndexForColor( pixelColor16 );
               }
            }
         }
      }
   }
}
