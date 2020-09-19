// DrawRect
// Draw a rectangle at entity location.
// width height color

class DrawRect extends ModuleBase
{
	constructor( parent,width,height,color )
	{
		super( parent )
		this.width = width
		this.height = height
		this.color = color
	}
	
	OnDraw()
	{
		gfx.DrawRect( this.parent.x - this.width / 2,
			this.parent.y - this.height / 2,
			this.width,this.height,
			this.color )
	}
}
