using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using DW3ArduinoEditor.Enums;
using DW3ArduinoEditor.Graphics;
using DW3ArduinoEditor.ViewModels;

namespace DW3ArduinoEditor
{
   public class TileMapPanel : FrameworkElement
   {
      private const int _defaultTileSize = Constants.TileSizePixels;

      private static readonly TimeSpan _zoomAnimationDuration = TimeSpan.FromMilliseconds( 500 );
      private static readonly EasingFunctionBase _zoomAnimationEase = new CircleEase
      {
         EasingMode = EasingMode.EaseOut
      };

      private readonly SolidColorBrush _background = new SolidColorBrush( Color.FromRgb( 64, 64, 64 ) );
      private readonly SolidColorBrush _highlight = new SolidColorBrush( Color.FromArgb( 128, 255, 0, 0 ) );

      private bool _isLeftButtonDown;
      private Point _dragAnchorPoint;

      private WriteableBitmap? _bitmap;
      private byte[]? _rawBuffer;

      private bool _isAnimatingZooming;

      private int _cellX;
      private int _cellY;

      private static readonly double[] _zoomTable = { 0.2, 0.3, 0.5, 0.8, 1, 1.5, 2, 3, 4, 5 };
      private static int _zoomLevel = 4;

      private Enums.InputMode _inputMode;

      private static readonly DependencyProperty ZoomProperty = DependencyProperty.Register(
         nameof( Zoom ),
         typeof( double ),
         typeof( TileMapPanel ),
         new FrameworkPropertyMetadata( _zoomTable[_zoomLevel], FrameworkPropertyMetadataOptions.AffectsRender ) );

      private double Zoom
      {
         get => (double)GetValue( ZoomProperty );
         set => SetValue( ZoomProperty, value );
      }

      private static readonly DependencyProperty TileHighlightProperty = DependencyProperty.Register(
         nameof( TileHighlight ),
         typeof( Rect ),
         typeof( TileMapPanel ),
         new FrameworkPropertyMetadata( Rect.Empty, FrameworkPropertyMetadataOptions.AffectsRender ) );

      public Rect TileHighlight
      {
         get => (Rect)GetValue( TileHighlightProperty );
         set => SetValue( TileHighlightProperty, value );
      }

      private static readonly DependencyProperty OffsetProperty = DependencyProperty.Register(
         nameof( Offset ),
         typeof( Vector ),
         typeof( TileMapPanel ),
         new FrameworkPropertyMetadata( new Vector( 0, 0 ), FrameworkPropertyMetadataOptions.AffectsRender ) );

      public Vector Offset
      {
         get => (Vector)GetValue( OffsetProperty );
         set => SetValue( OffsetProperty, value );
      }

      public static readonly DependencyProperty SelectedTileMapProperty = DependencyProperty.Register(
         nameof( SelectedTileMap ),
         typeof( TileMapViewModel ),
         typeof( TileMapPanel ),
         new PropertyMetadata( OnSelectedTileMapChanged ) );

      public TileMapViewModel SelectedTileMap
      {
         get => (TileMapViewModel)GetValue( SelectedTileMapProperty );
         set => SetValue( SelectedTileMapProperty, value );
      }

      private static void OnSelectedTileMapChanged( DependencyObject obj, DependencyPropertyChangedEventArgs e )
      {
         if ( obj is not TileMapPanel sender || e.NewValue is not TileMapViewModel tileMap )
         {
            return;
         }

         sender.PrepareBitmap();
      }

      public static readonly DependencyProperty TileTexturePoolProperty = DependencyProperty.Register(
         nameof( TileTexturePool ),
         typeof( TileTexturePool ),
         typeof( TileMapPanel ),
         new PropertyMetadata( OnTileTexturePoolChanged ) );

      public TileTexturePool TileTexturePool
      {
         get => (TileTexturePool)GetValue( TileTexturePoolProperty );
         set => SetValue( TileTexturePoolProperty, value );
      }

      private static void OnTileTexturePoolChanged( DependencyObject obj, DependencyPropertyChangedEventArgs e )
      {
         if ( obj is not TileMapPanel sender || e.NewValue is not TileTexturePool tileTexturePool )
         {
            return;
         }

         sender.PrepareBitmap();
      }

      public static readonly DependencyProperty StaticSpriteTexturePoolProperty = DependencyProperty.Register(
         nameof( StaticSpriteTexturePool ),
         typeof( StaticSpriteTexturePool ),
         typeof( TileMapPanel ),
         new PropertyMetadata( OnStaticSpriteTexturePoolChanged ) );

      public StaticSpriteTexturePool StaticSpriteTexturePool
      {
         get => (StaticSpriteTexturePool)GetValue( StaticSpriteTexturePoolProperty );
         set => SetValue( StaticSpriteTexturePoolProperty, value );
      }

      private static void OnStaticSpriteTexturePoolChanged( DependencyObject obj, DependencyPropertyChangedEventArgs e )
      {
         if ( obj is not TileMapPanel sender || e.NewValue is not StaticSpriteTexturePool staticSpriteTexturePool )
         {
            return;
         }

         sender.PrepareBitmap();
      }

      public static readonly DependencyProperty SelectedTileTextureSetProperty = DependencyProperty.Register(
         nameof( SelectedTileTextureSet ),
         typeof( TextureSetViewModel ),
         typeof( TileMapPanel ),
         new PropertyMetadata( OnSelectedTileTextureSetChanged ) );

      public TextureSetViewModel SelectedTileTextureSet
      {
         get => (TextureSetViewModel)GetValue( SelectedTileTextureSetProperty );
         set => SetValue( SelectedTileTextureSetProperty, value );
      }

      private static void OnSelectedTileTextureSetChanged( DependencyObject obj, DependencyPropertyChangedEventArgs e )
      {
         if ( obj is not TileMapPanel sender || e.NewValue is not TextureSetViewModel selectedTileTextureSet )
         {
            return;
         }

         sender.PrepareBitmap();
      }

      public static readonly DependencyProperty SelectedStaticSpriteTextureSetProperty = DependencyProperty.Register(
         nameof( SelectedStaticSpriteTextureSet ),
         typeof( TextureSetViewModel ),
         typeof( TileMapPanel ),
         new PropertyMetadata( OnSelectedStaticSpriteTextureSetChanged ) );

      public TextureSetViewModel SelectedStaticSpriteTextureSet
      {
         get => (TextureSetViewModel)GetValue( SelectedTileTextureSetProperty );
         set => SetValue( SelectedTileTextureSetProperty, value );
      }

      private static void OnSelectedStaticSpriteTextureSetChanged( DependencyObject obj, DependencyPropertyChangedEventArgs e )
      {
         if ( obj is not TileMapPanel sender || e.NewValue is not TextureSetViewModel selectedTileTextureSet )
         {
            return;
         }

         sender.PrepareBitmap();
      }

      public static readonly DependencyProperty SelectedTileTextureIndexProperty = DependencyProperty.Register(
         nameof( SelectedTileTextureIndex ),
         typeof( int ),
         typeof( TileMapPanel ) );

      public int SelectedTileTextureIndex
      {
         get => (int)GetValue( SelectedTileTextureIndexProperty );
         set => SetValue( SelectedTileTextureIndexProperty, value );
      }

      public static readonly DependencyProperty SelectedTileEditToolProperty = DependencyProperty.Register(
         nameof( SelectedTileEditTool ),
         typeof( TileEditTool ),
         typeof( TileMapPanel ) );

      public TileEditTool SelectedTileEditTool
      {
         get => (TileEditTool)GetValue( SelectedTileEditToolProperty );
         set => SetValue( SelectedTileEditToolProperty, value );
      }

      public static readonly DependencyProperty ShowStaticSpritesProperty = DependencyProperty.Register(
         nameof( ShowStaticSprites ),
         typeof( bool ),
         typeof( TileMapPanel ),
         new PropertyMetadata( OnShowStaticSpritesChanged ) );

      public bool ShowStaticSprites
      {
         get => (bool)GetValue( ShowStaticSpritesProperty );
         set => SetValue( ShowStaticSpritesProperty, value );
      }

      private static void OnShowStaticSpritesChanged( DependencyObject obj, DependencyPropertyChangedEventArgs e )
      {
         if ( obj is not TileMapPanel sender || e.NewValue is not bool showStaticSprites )
         {
            return;
         }

         sender.PrepareBitmap();
      }

      public TileMapPanel()
      {
         ClipToBounds = true;
         Focusable = true;
      }

      /// <summary>
      /// Gets the "current" size of the tiles in the UI. This accounts for the various UI states,
      /// such as pan and zoom.
      /// </summary>
      /// <returns>A size instances that describes the pixel size of the tiles in the UI.</returns>
      private Size GetCurrentTileSize()
      {
         return new Size( _defaultTileSize * Zoom, _defaultTileSize * Zoom );
      }

      // TODO: this could be more efficient, we could draw only the portion of the map that's visible
      private void PrepareBitmap()
      {
         if ( TileTexturePool is null || SelectedTileTextureSet is null || SelectedTileMap is null )
         {
            return;
         }

         const int bytesPerPixel = 4;

         int tilesPerRow = (int)SelectedTileMap.TilesX;
         int tilesPerCol = (int)SelectedTileMap.TilesY;

         int width = tilesPerRow * _defaultTileSize * bytesPerPixel;
         int height = tilesPerCol * _defaultTileSize * bytesPerPixel;

         _bitmap = new WriteableBitmap( width, height, 96, 96, PixelFormats.Bgra32, null );
         _rawBuffer = new byte[width * height];

         for ( int i = 0; i < SelectedTileMap.Tiles.Count; i++ )
         {
            int cellX = i % tilesPerRow;
            int cellY = i / tilesPerRow;

            int destX = cellX * _defaultTileSize * bytesPerPixel;
            int destY = cellY * _defaultTileSize;

            int textureIndex = (int)SelectedTileMap.Tiles[i].TextureIndex;
            var tileSprite = TileTexturePool.GetSpriteFromIndex( SelectedTileTextureSet.TexturePoolIndexes[(int)SelectedTileMap.Tiles[i].TextureIndex] );

            tileSprite.DrawToBuffer( _rawBuffer, width, destX, destY );
         }

         if ( ShowStaticSprites && StaticSpriteTexturePool is not null && SelectedStaticSpriteTextureSet is not null )
         {
            foreach ( var s in SelectedTileMap.StaticSprites )
            {
               int cellX = (int)s.TileIndex % tilesPerRow;
               int cellY = (int)s.TileIndex / tilesPerRow;

               int destX = cellX * _defaultTileSize * bytesPerPixel;
               int destY = cellY * _defaultTileSize;

               var staticSprite = StaticSpriteTexturePool.GetSpriteFromIndex( s.TextureIndex );
               staticSprite.DrawToBuffer( _rawBuffer, width, destX, destY );
            }
         }

         // I don't know why I have to divide by 4 on the width, height
         // Steve note: it's probably something to do with the "stride" value, you're
         // sending in the width, but I think they want the actual number of bytes per line.
         // that would be width * (bpp / 8), so width * 4, although if you change that you'd
         // also need to update other stuff in here, like the size of _rawBuffer.
         _bitmap.WritePixels( new Int32Rect( 0, 0, _bitmap.PixelWidth / 4, _bitmap.PixelHeight / 4 ), _rawBuffer, _bitmap.PixelWidth, 0 );
      }

      // TODO: it would be cool if this also tried to center the view on the mouse cursor
      private void SetZoomLevel( int zoomFactorDelta )
      {
         if ( _isAnimatingZooming )
         {
            return;
         }

         _isAnimatingZooming = true;
         _zoomLevel = Math.Clamp( _zoomLevel + zoomFactorDelta, 0, _zoomTable.Length - 1 );
         double zoomFactor = _zoomTable[_zoomLevel];

         var zoomLevelAnimation = new DoubleAnimation( zoomFactor, _zoomAnimationDuration )
         {
            EasingFunction = _zoomAnimationEase
         };

         zoomLevelAnimation.Completed += OnAnimationComplete;
         BeginAnimation( ZoomProperty, zoomLevelAnimation );

         void OnAnimationComplete( object? sender, EventArgs e )
         {
            _isAnimatingZooming = false;
         }
      }

      protected override void OnPreviewKeyDown( KeyEventArgs e )
      {
         if ( e.Key == Key.Space )
         {
            _inputMode = Enums.InputMode.Pan;
            Cursor = Cursors.ScrollAll;
         }
      }

      protected override void OnPreviewKeyUp( KeyEventArgs e )
      {
         base.OnPreviewKeyUp( e );

         _inputMode = Enums.InputMode.Draw;
         Cursor = Cursors.Arrow;
      }

      protected override void OnMouseWheel( MouseWheelEventArgs e )
      {
         base.OnMouseWheel( e );

         int delta = e.Delta > 0 ? 1 : -1;
         SetZoomLevel( delta );
      }

      protected override void OnMouseEnter( MouseEventArgs e )
      {
         base.OnMouseEnter( e );

         // mousing over the control steals focus so the keyboard events will work
         Focus();
      }

      protected override void OnMouseLeftButtonDown( MouseButtonEventArgs e )
      {
         base.OnMouseLeftButtonDown( e );

         _isLeftButtonDown = true;

         switch ( _inputMode )
         {
            case Enums.InputMode.Draw:
               {
                  if ( SelectedTileEditTool == TileEditTool.ChangeTexture )
                  {
                     ChangeTileTexture();
                  }
                  break;
               }

            case Enums.InputMode.Pan:
               {
                  CaptureMouse();
                  _dragAnchorPoint = e.GetPosition( this ) - Offset;

                  break;
               }
         }
      }

      protected override void OnMouseMove( MouseEventArgs e )
      {
         base.OnMouseMove( e );

         if ( _bitmap is null )
         {
            return;
         }

         var mousePos = e.GetPosition( this );
         var mapRect = new Rect( Offset.X * Zoom, Offset.Y * Zoom, _bitmap.Width, _bitmap.Height );

         if ( mapRect.Contains( mousePos ) )
         {
            var tileSize = GetCurrentTileSize();
            _cellX = (int)( ( mousePos.X - Offset.X * Zoom ) / tileSize.Width );
            _cellY = (int)( ( mousePos.Y - Offset.Y * Zoom ) / tileSize.Height );

            TileHighlight = new Rect( _cellX * _defaultTileSize, _cellY * _defaultTileSize, _defaultTileSize, _defaultTileSize );
         }
         else
         {
            TileHighlight = Rect.Empty;
         }

         switch ( _inputMode )
         {
            case Enums.InputMode.Draw:
            {
               if ( _isLeftButtonDown && SelectedTileEditTool == TileEditTool.ChangeTexture )
               {
                  ChangeTileTexture();
               }
               break;
            }

         case Enums.InputMode.Pan:
               {
                  if ( _isLeftButtonDown )
                  {
                     Offset = mousePos - _dragAnchorPoint;
                  }
                  break;
               }
         }
      }

      protected override void OnMouseLeftButtonUp( MouseButtonEventArgs e )
      {
         base.OnMouseLeftButtonUp( e );
         ReleaseMouseCapture();

         _isLeftButtonDown = false;

         switch ( _inputMode )
         {
            case Enums.InputMode.Draw:
               {
                  if ( SelectedTileEditTool == TileEditTool.FloodFillTexture )
                  {
                     FloodFillTexture();
                  }
                  else if ( SelectedTileEditTool == TileEditTool.ShoreFill )
                  {
                     ShoreFill();
                  }
                  break;
               }
         }
      }

      protected override void OnRender( DrawingContext dc )
      {
         dc.DrawRectangle( _background, null, new Rect( 0, 0, ActualWidth, ActualHeight ) );

         if ( _bitmap is not null )
         {
            var transform = new TransformGroup();
            transform.Children.Add( new TranslateTransform( Offset.X, Offset.Y ) );
            transform.Children.Add( new ScaleTransform( Zoom, Zoom ) );

            dc.PushTransform( transform );
            dc.DrawImage( _bitmap, new Rect( 0, 0, _bitmap.Width, _bitmap.Height ) );

            if ( TileHighlight != Rect.Empty && _inputMode == Enums.InputMode.Draw )
            {
               dc.DrawRectangle( _highlight, null, TileHighlight );
            }

            dc.Pop();
         }
      }

      private void ChangeTileTexture()
      {
         if ( !CanEditTileMap() )
         {
            return;
         }

         int tilesPerRow = (int)SelectedTileMap.TilesX;
         int offset = _cellY * tilesPerRow + _cellX;
         var tileViewModel = SelectedTileMap.Tiles[offset];

         if ( tileViewModel.TextureIndex == SelectedTileTextureIndex )
         {
            // no need to redraw the tile if it's the same
            return;
         }

         tileViewModel.TextureIndex = (uint)SelectedTileTextureIndex;
         DrawTileBitmap( SelectedTileTextureIndex, _cellX, _cellY, _bitmap );
         InvalidateVisual();
      }

      private void FloodFillTexture()
      {
         if ( !CanEditTileMap() )
         {
            return;
         }

         var targetTexture = SelectedTileMap.Tiles[(int)( _cellY * SelectedTileMap.TilesX ) + _cellX].TextureIndex;

         if ( targetTexture == SelectedTileTextureIndex )
         {
            // No need to flood fill if the tile is already the same
            return;
         }

         var fillTexture = (uint)SelectedTileTextureIndex;

         // NOTE: this flood fill algorithm was AI-generated, so take it with a grain of salt!

         // use a Queue for iterative flood fill (BFS) to avoid stack overflow issues
         var q = new Queue<( int, int )>();
         q.Enqueue( ( _cellY, _cellX ) );

         // change the origin tile texture
         int tileIndex = (int)( ( _cellY * SelectedTileMap.TilesX ) + _cellX );
         SelectedTileMap.Tiles[tileIndex].TextureIndex = fillTexture;
         DrawTileBitmap( (int)fillTexture, _cellX, _cellY, _bitmap );

         // Define the 4-way direction vectors (up, down, left, right)
         int[] dr = { -1, 1, 0, 0 };
         int[] dc = { 0, 0, -1, 1 };

         while ( q.Count > 0 )
         {
            ( int r, int c ) = q.Dequeue();

            // check neighbors in all four directions
            for ( int i = 0; i < 4; i++ )
            {
               int nr = r + dr[i]; // neighbor row
               int nc = c + dc[i]; // neighbor column

               // check if the neighbor is within bounds
               if ( nr >= 0 && nr < SelectedTileMap.TilesY && nc >= 0 && nc < SelectedTileMap.TilesX )
               {
                  // check if the neighbor has the target texture
                  tileIndex = (int)( nr * SelectedTileMap.TilesX ) + nc;

                  if ( SelectedTileMap.Tiles[tileIndex].TextureIndex == targetTexture )
                  {
                     // fill the neighbor and add it to the queue
                     SelectedTileMap.Tiles[tileIndex].TextureIndex = fillTexture;
                     DrawTileBitmap( (int)fillTexture, nc, nr, _bitmap );
                     q.Enqueue( ( nr, nc ) );
                  }
               }
            }
         }

         InvalidateVisual();
      }

      // TODO: maybe figure out a way to combine this with FloodFillTexture()?
      private void ShoreFill()
      {
         if ( !CanEditTileMap() )
         {
            return;
         }

         if ( !TileTextureIsWater( (int)SelectedTileMap.Tiles[(int)( _cellY * SelectedTileMap.TilesX ) + _cellX].TextureIndex ) )
         {
            // didn't click on water, no need to try filling in the shore
            return;
         }

         // use a Queue for iterative flood fill (BFS) to avoid stack overflow issues
         var q = new Queue<( int, int )>();
         q.Enqueue( ( _cellY, _cellX ) );
         var alreadyChecked = new List<( int, int )>();

         // Define the 4-way direction vectors (up, down, left, right)
         int[] dr = { -1, 1, 0, 0 };
         int[] dc = { 0, 0, -1, 1 };

         while ( q.Count > 0 )
         {
            ( int r, int c ) = q.Dequeue();

            if ( alreadyChecked.Contains( ( r, c ) ) )
            {
               continue;
            }

            bool createShore = false;

            // check neighbors in all four directions
            for ( int i = 0; i < 4; i++ )
            {
               int nr = r + dr[i]; // neighbor row
               int nc = c + dc[i]; // neighbor column

               if ( alreadyChecked.Contains( ( nr, nc ) ) )
               {
                  continue;
               }

               // check if the neighbor is within bounds
               if ( nr >= 0 && nr < SelectedTileMap.TilesY && nc >= 0 && nc < SelectedTileMap.TilesX )
               {
                  if ( TileTextureIsWater( (int)( nr * SelectedTileMap.TilesX ) + nc ) )
                  {
                     // neighbor is water, add to the queue
                     q.Enqueue( ( nr, nc ) );
                  }
                  else
                  {
                     // neighbor is not water, this tile needs to be a shore
                     createShore = true;
                  }
               }
            }

            if ( createShore )
            {
               CreateShore( r, c );
            }

            alreadyChecked.Add( (r, c) );
         }

         InvalidateVisual();
      }

      private bool TileTextureIsWater( int tileIndex ) => ( SelectedTileMap.Tiles[tileIndex].TextureIndex == SelectedTileTextureSet.WaterTextureIndex ||
                                                            SelectedTileMap.Tiles[tileIndex].TextureIndex == SelectedTileTextureSet.ShoalTextureIndex ||
                                                            ( SelectedTileTextureSet.ShoreTextureStartIndex >= 0 &&
                                                              SelectedTileMap.Tiles[tileIndex].TextureIndex >= SelectedTileTextureSet.ShoreTextureStartIndex &&
                                                              SelectedTileMap.Tiles[tileIndex].TextureIndex < SelectedTileTextureSet.ShoreTextureStartIndex + (uint)ShoreType.Count ) );

      private bool TileTextureIsWaterOrFloating( int tileIndex ) => TileTextureIsWater( tileIndex ) ||
                                                                    SelectedTileMap.Tiles[tileIndex].TextureIndex == SelectedTileTextureSet.HorizontalBridgeTextureIndex ||
                                                                    SelectedTileMap.Tiles[tileIndex].TextureIndex == SelectedTileTextureSet.VerticalBridgeTextureIndex;

      private void CreateShore( int row, int col )
      {
         int tileIndex = (int)( row * SelectedTileMap.TilesX ) + col;

         bool leftIsWater = ( col > 0 ) ? TileTextureIsWaterOrFloating( tileIndex - 1 ) : true;
         bool topIsWater = ( row > 0 ) ? TileTextureIsWaterOrFloating( tileIndex - (int)SelectedTileMap.TilesX ) : true;
         bool rightIsWater = ( col < SelectedTileMap.TilesX - 1 ) ? TileTextureIsWaterOrFloating( tileIndex + 1 ) : true;
         bool bottomIsWater = ( row < SelectedTileMap.TilesY - 1 ) ? TileTextureIsWaterOrFloating( tileIndex + (int)SelectedTileMap.TilesY ) : true;

         string enumValue = string.Format( "{0}{1}{2}{3}", leftIsWater ? string.Empty : "Left", topIsWater ? string.Empty : "Top", rightIsWater ? string.Empty : "Right", bottomIsWater ? string.Empty : "Bottom" );

         if ( !Enum.TryParse( enumValue, out ShoreType shoreType ) )
         {
            shoreType = ShoreType.LeftTopRightBottom;
         }

         var textureIndex = SelectedTileTextureSet.ShoreTextureStartIndex + (int)shoreType;
         SelectedTileMap.Tiles[tileIndex].TextureIndex = (uint)textureIndex;
         DrawTileBitmap( (int)textureIndex, col, row, _bitmap );
      }

      private bool CanEditTileMap()
      {
         if ( _bitmap is null || SelectedTileMap is null || TileTexturePool is null || SelectedTileTextureSet is null ||
              _cellX < 0 || _cellX >= SelectedTileMap.TilesX || _cellY < 0 || _cellY >= SelectedTileMap.TilesY )
         {
            return false;
         }

         if ( ( SelectedTileEditTool == TileEditTool.ChangeTexture || SelectedTileEditTool == TileEditTool.FloodFillTexture ) &&
              ( SelectedTileTextureIndex < 0 || SelectedTileTextureIndex >= SelectedTileTextureSet.TexturePoolIndexes.Count ) )
         {
            return false;
         }

         return true;
      }

      private void DrawTileBitmap( int textureIndex, int cellX, int cellY, WriteableBitmap? bitmap )
      {
         if ( bitmap is null )
         {
            return;
         }

         var byteBuffer = new byte[_defaultTileSize * _defaultTileSize * 4];
         var tileSprite = TileTexturePool.GetSpriteFromIndex( SelectedTileTextureSet.TexturePoolIndexes[textureIndex] );
         tileSprite.DrawToBuffer( byteBuffer, _defaultTileSize * 4, 0, 0 );

         int destX = cellX * Constants.TileSizePixels;
         int destY = cellY * Constants.TileSizePixels;

         bitmap.WritePixels( new Int32Rect( destX, destY, _defaultTileSize, _defaultTileSize ), byteBuffer, _defaultTileSize * 4, 0 );
      }
   }
}
